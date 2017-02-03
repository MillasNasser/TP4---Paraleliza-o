#include "func.h"
#define teste(a) printf("%d teste\n",a);
#define imPes(a) printf("%d|%d:%d:%lu:%lu|%d\n",a->anterior == NULL,a->chama_se,a->vou_para,a->jep_entra,a->jep_saida,a->proximo == NULL);
#define imPes2(a) printf("%d:%d:%lu:%lu\n",a->chama_se,a->vou_para,a->jep_entra,a->jep_saida);

int go;
descritor *inicio;//Vetor que simula os andares
pthread_mutex_t acssGo, acssInicio;

int sairElevador( int cap,int andar,int *in, unsigned long int *jepsy, pessoa* capTotal){
	pessoa *aux = (pessoa*)malloc(sizeof(pessoa));//Auxilia na caminhada pela lista
	int i=0,j=0;
	for(j=cap;j>0;j--){///Verificação para descobrir se há passageiros para sair
		if(capTotal[j-1].proximo!=NULL && capTotal[j-1].proximo->vou_para==andar+1){//Caso houver algum passageiro que deseja sair nesse andar, remove-o
			i=1; (*in)--;
			aux=capTotal[j-1].proximo;
			aux->jep_saida = (*jepsy)+1;
			inicio[aux->chama_se-1].quantidade--;
			capTotal[j-1].proximo = NULL;
			if(inicio[aux->chama_se-1].quantidade==0){
				pthread_mutex_lock(&acssGo);
				go--;
				pthread_mutex_unlock(&acssGo);
			}
		}
	}
	free(aux);
	return i;
}

int entrarElevador(int cap, int andar, int sob_des, int *in, unsigned long int jepsy, pessoa *capTotal, int nThread){
	int i,j=0;
	pessoa *aux = (pessoa*)malloc(sizeof(pessoa));
	while(inicio[andar].quantidade!=0 && (*in)!=cap && aux!=NULL){///Se o andar não estiver nulo e haver espaço no elevador
		aux = inicio[andar].primeiro;//Auxiliar de passagem pela lista
		if(pthread_mutex_lock(&acssInicio)!=0){continue;}
		while(capTotal[j].proximo==NULL && aux!=NULL){///Busca de posição nula e nodo que não foi inserido ainda
			if((sob_des==1) && (aux->vou_para > aux->chama_se) && (aux->jep_entra==0)){//Se o elevador sobe, a pessoa for subir e não foi chamada
				capTotal[j].proximo = aux;
				aux->jep_entra = jepsy+1;
				(*in)++; i=1;
			}
			else if((sob_des==0) && (aux->vou_para < aux->chama_se) && (aux->jep_entra==0)){//Se o elevador desce, a pessoa for descer e não foi chamada
				capTotal[j].proximo = aux;
				aux->jep_entra = jepsy+1;
				(*in)++; i=1;
			}
			aux = aux->proximo;
		}
		j=(j+1)%cap;
		pthread_mutex_unlock(&acssInicio);
	}
	return i;
}

void* M1Paralelo(void *data){
	capM1 dados = *(p_capM1)data;
	int cap = dados.cap, nAndar = dados.nAndar;

	int in=0;
	int sob_des = 1;//mostra o sentido do elevador 1==Subir 0==descer
	unsigned long int jepsy=0;//realiza a contagem dos jepsyons
	int andar=dados.andar;//realiza a contagem dos andares
	pessoa *capTotal = (pessoa*) calloc(sizeof(pessoa),cap);//Vetor que simula a capacidade do elevador

	while(go>0){///Inicio da parte lógica do elevador
		int i=0,j;

		/*for(j=0;j<cap;j++){
			printf("%d: %d\n", dados.nThread, capTotal->proximo == NULL);
		}printf("\n");*/

		if(sob_des == 1 && andar == nAndar-1){///Realiza a mudança de sentido do elevador
			sob_des = 0;
		}else
		if(sob_des == 0 && andar == 0){
			sob_des = 1;
		}

		i = sairElevador(cap,andar,&in,&jepsy,capTotal);
		FILE*entrad = fopen("underline.txt","w");
			fprintf(stdout,"-\n");
		fclose(entrad);
		//usleep(1000);
		i = entrarElevador(cap,andar,sob_des,&in,jepsy,capTotal,dados.nThread);

		if(i==1) jepsy++;
		if(sob_des == 1) andar++;
		else andar--;
		jepsy++;
	}
	free(capTotal);
	return NULL;
}

///################################################################################
///################################################################################
///################################################################################
void metodo1(char evento[100], char elevar[100], int threads){//Método otimizado
	int in=0, cap, nAndar, i, j;
	FILE *eventos = fopen(evento,"r");
	FILE *elevador = fopen(elevar,"r");
	p_capM1 data = (p_capM1)calloc(sizeof(capM1),1);//Vetor capsula de dados
	pthread_t *vetorThread = (pthread_t*)calloc(sizeof(pthread_t),threads);//Vetor de threads

	if(!fscanf(elevador,"%d%d", &cap, &nAndar)){exit(1);}//Leitura dos dados do elevador
	inicio = (descritor*) calloc(sizeof(descritor),nAndar);//Vetor que simula os andares
	go=0;

	if(pthread_mutex_init(&acssGo,NULL)){printf("erro iniciar acssGO\n");}
	if(pthread_mutex_init(&acssInicio,NULL)){printf("erro iniciar acssInicio\n");}

	//Realização das adiçoes dos eventos(Pessoas)
	do{///Inserção de todos os nodos.
		if(in!=go) insert2(&inicio[in-1],in,go);
		if(!fscanf(eventos,"%d",&in)){exit(1);}
		if(!fscanf(eventos,"%d",&go)){exit(1);}
	}while(!feof(eventos));

	for(in=nAndar,go=0;in>0;in--){//Contagem de andares não nulos
		if(inicio[in-1].quantidade!=0) go++;
	}

	data->andar = 0;
	data->cap = cap;
	data->nAndar = nAndar;
	for(i = 0; i<threads; i++){
		data->nThread = i;
		pthread_create(&(vetorThread[i]),NULL,M1Paralelo,data);
		usleep(5000); //Timming para disparar todos os threads corretamente
	}

	for(i = 0; i<threads; i++){
		pthread_join(vetorThread[i],NULL);
	}

	for(i=0;i<nAndar;i++){
		pessoa *aux = inicio[i].primeiro;

		while(aux!=NULL){
			printf("(%d..%d): %lu, %lu\n",aux->chama_se,aux->vou_para,aux->jep_entra,aux->jep_saida);
			aux = aux->proximo;
		}
	}

	fclose(eventos);
	fclose(elevador);
	free(inicio);
return;
}

int main(int argc, char** argv){
	metodo1(argv[1],argv[2],atoi(argv[3]));


return 0;
}

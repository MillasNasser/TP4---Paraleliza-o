#include "func.h"

#define numThread 2

descritor *head = NULL;
pthread_t vetThread1;
pthread_t vetThread2;
int flagF_EOF;

void* M2Paralelo(void *data){
	printf("entrei M2\n");
	usleep(1000);
	unsigned long int i = 0;
	p_capM2 distribuir = (p_capM2)data;
	int qntChaves = distribuir->qntChaves;
	pessoa *comeco = distribuir->comeco->proximo;
	pessoa *auxiliar = (pessoa*)calloc(sizeof(pessoa),1);
	unsigned long int jepsy=1;

	auxiliar->chama_se = 1;
	auxiliar->vou_para = 1;
	auxiliar->proximo = (pessoa*)malloc(sizeof(pessoa));

	while(flagF_EOF!=-1){/*comeco!=NULL && comeco->jep_entra == 0 && qntChaves>0*/
		if(auxiliar->proximo == NULL && comeco==NULL){
			//flagF_EOF = -1;
			continue;
		}else if(auxiliar->proximo != NULL && comeco==NULL){
			comeco = auxiliar->proximo;
		}

		if(auxiliar->vou_para != comeco->chama_se){
			jepsy += abs(auxiliar->vou_para - comeco->chama_se) + 1;
		}
		comeco->jep_entra = jepsy;
		jepsy += abs(comeco->chama_se - comeco->vou_para);
		jepsy++;
		comeco->jep_saida = jepsy;
		auxiliar = comeco;

		comeco = comeco->proximo;
	}
return NULL;
}

void* escritaParalela(void* data){
	pessoa *marcador = (pessoa*)data;
	marcador = marcador->proximo;
	printf("entro na escrita paralela\n");
	FILE *saidaM2 = fopen("metodo2.txt","w"); //Saida do metodo
	///Possivel paralelização #3
	while(marcador!=NULL){
		printf("(%d..%d): %lu, %lu\n",marcador->chama_se,marcador->vou_para,marcador->jep_entra,marcador->jep_saida);
		fprintf(saidaM2,"(%d..%d): %lu, %lu\n",marcador->chama_se,marcador->vou_para,marcador->jep_entra,marcador->jep_saida);
		marcador = marcador->proximo;
	}
return NULL;
}

void* leituraParalela(void* data){
	printf("entrei R\n");
	int qnt=-1;
	int in,go;
	char *evento = (char*)data;
	printf("leitura: antes do\n");
	FILE *eventos = fopen(evento,"r");
	if(eventos == NULL){
		printf("error\n");
	}
	printf("leitura: antes do\n");
	head = (descritor*)calloc(sizeof(descritor),1);
	do{///Inserção de todos os nodos.
		qnt++;
		printf("%d\n",qnt);
		if(in!=go) insert(head,in,go);
		if(!fscanf(eventos,"%d",&in)){exit(1);}
		if(!fscanf(eventos,"%d",&go)){exit(1);}
	}while(!feof(eventos));
	printf("Sai leitura\n");
	flagF_EOF = -1;
return NULL;
}

void metodo2(char evento[100],char elevar[100],int threads){//Lista simples e ordenada.
	int i, j, in=0, go=0, cap=0, qnt = -1;
	int nAndar=0;
	FILE* elevador = fopen(elevar,"r");
	p_capM2 capsula = (p_capM2)malloc(sizeof(capM2));
	//vetThread = (pthread_t*)calloc(sizeof(pthread_t),numThread);
	pessoa *marcador = NULL;
	flagF_EOF = 0;

	printf("antes dos threads\n");

	if(!fscanf(elevador,"%d%d",&cap,&nAndar)){exit(1);}
	if(cap<=0 || nAndar < 1){exit(0);}

	usleep(1000);
	pthread_create(&vetThread1,NULL,leituraParalela,(void*)evento);

	printf("enquanto head==NULL\n");
	while(head->primeiro==NULL){printf("depois do while\n");}
	//printf("depois do while\n");
	marcador = head->primeiro;

	//Pega o teto da divisão qnt/threads
	/*capsula->qntChaves = qnt/threads;
	if(threads>0 && qnt/threads>(int)qnt/threads){
		capsula->qntChaves = (int)qnt/threads+1;
	}*/
	sleep(2);
	while(head->primeiro!=NULL){
		printf("%d %d %li %li\n",head->primeiro->vou_para,head->primeiro->chama_se,head->primeiro->jep_entra,head->primeiro->jep_saida);
	}

	capsula->comeco = head->primeiro;
	printf("antes do usleep\n");
	usleep(5000);
	//pthread_create(&vetThread2,NULL,M2Paralelo,(void*)capsula);
	printf("antes pthread join\n");
	pthread_join(vetThread1,NULL);
	pthread_join(vetThread2,NULL);
	printf("depois pthread join\n");
	//pthread_join(vetThread[1],NULL);
	escritaParalela((void*)marcador);

	//fclose(eventos);
	//fclose(elevador);
	//fclose(saidaM2);
return;
}

int main(int argc, char** argv){
	metodo2(argv[1],argv[2],atoi(argv[3]));
return 0;
}

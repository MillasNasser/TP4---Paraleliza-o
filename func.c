/* arquivo_lógico - Simulador de Elevador
 *
 * Millas Násser
 * 05/set/2016
 * */
#include "func.h"

#define teste(a) printf("%d teste\n",a);
#define imPes(a) printf("%d|%d:%d:%lu:%lu|%d\n",a->anterior == NULL,a->chama_se,a->vou_para,a->jep_entra,a->jep_saida,a->proximo == NULL);
#define imPes2(a) printf("%d:%d:%lu:%lu\n",a->chama_se,a->vou_para,a->jep_entra,a->jep_saida);
//#define

void impList(pessoa *lista){
	pessoa *aux = lista;
	while(aux!=NULL){
		printf("{%d|%d|%lu|%lu}\n",aux->chama_se,aux->vou_para,aux->jep_entra,aux->jep_saida);
		aux = aux->proximo;
	}
}

void cria_lista(descritor *cabeca){///Inicializa a lista
	cabeca->quantidade = 0;
	cabeca->primeiro = NULL;
	cabeca->ultimo = NULL;
}

void limpa_lista(descritor *head){
	pessoa *aux = (pessoa*) malloc(sizeof(pessoa));
	head->ultimo = NULL;
	while(head->primeiro!=NULL){
		aux = head->primeiro;
		head->primeiro = head->primeiro->proximo;
		free(aux);
		head->quantidade--;
	}
}

void pega_flag(int argc, char **argv, char parg[100], char earg[100], int *threads){
	int verbose;
	opterr = 0;
	while((verbose = getopt(argc,argv,"e:p:T:")) != -1){
		switch(verbose){
			case 'e':
				strcpy(earg,optarg);
			break;
			case 'p':
				strcpy(parg,optarg);
			break;
			case 'T':
				*threads = atoi(optarg);
			break;
			case '?':
				if (optopt == 'p'){
					printf("Opcao \"%c\" necessita de mais argumentos!\n", optopt);
				}else if( optopt == 'e' ){
					printf("Opcao \"%c\" necessita de mais argumentos!\n", optopt);
				}else if( optopt == 'T' ){
					printf("Opcao \"%c\" necessita de mais argumentos!\n", optopt);
				}else{
					printf("Opcao \"%c\" desconhecida!\n",optopt);
				}
			return;
		}
	}
}/**/

void insert2(descritor *head, int in, int go){///Insere nodo na lista
	pessoa *nodo = (pessoa *)malloc(sizeof(pessoa));
	pessoa *auxi = head->primeiro;

	nodo->chama_se = in;
	nodo->vou_para = go;
	nodo->jep_entra = 0;
	nodo->jep_saida = 0;
	nodo->proximo = NULL;

	if(head->quantidade==0){
		nodo->anterior = NULL;
		head->primeiro = nodo;
		head->ultimo = nodo;
		head->quantidade++;
	}else{
		while( auxi!=NULL && go > auxi->vou_para ){
			auxi = auxi->proximo;
		}
		head->quantidade++;
		if(auxi==NULL){
			nodo->anterior = head->ultimo;
			head->ultimo->proximo = nodo;
			head->ultimo = nodo;
		}else{
			nodo->proximo = auxi;
			nodo->anterior = auxi->anterior;
			auxi->anterior = nodo;
			if(nodo->anterior==NULL){
				head->primeiro = nodo;
				return;
			}
			nodo->anterior->proximo = nodo;
		}
	}
}

void insert(descritor *head, int in, int go){///Insere nodo na lista
	pessoa *nodo = (pessoa *)calloc(sizeof(pessoa),1);

	nodo->chama_se = in;
	nodo->vou_para = go;
	nodo->jep_entra = 0;
	nodo->jep_saida = 0;
	nodo->proximo= NULL;

	if(head->quantidade==0){
		nodo->anterior = NULL;
		head->primeiro = nodo;
		head->ultimo = nodo;
		head->quantidade++;
	}else{
		nodo->anterior = head->ultimo;
		head->ultimo->proximo = nodo;
		head->ultimo = nodo;
		head->quantidade++;
	}
}

int abs(int num){
	if(num<0){
		num *= -1;
	}
return num;
}

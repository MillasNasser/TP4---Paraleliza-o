/* biblioteca - Simulador de Elevador
 *
 * Millas Násser
 * 05/set/2016
 * */
#ifndef HEAD_C
#define HEAD_C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

typedef struct s_pessoa{///Estrutura para pessoas
	struct s_pessoa *anterior;
	int chama_se;
	int vou_para;
	unsigned long int jep_entra;
	unsigned long int jep_saida;
	struct s_pessoa *proximo;
}pessoa;

typedef struct s_descritor{///Descritor para pessoas
	int quantidade;
	pessoa *primeiro;
	pessoa *ultimo;
}descritor;

typedef struct scapsulaMetodo1{
	int cap;
	int nAndar;
	int andar;
	int nThread;
}capM1,*p_capM1;

typedef struct scapsulaMetodo2{
	pessoa *comeco;
	int qntChaves;
}capM2,*p_capM2;

void cria_lista(descritor *cabeca);
void insert(descritor *head, int in, int go);
void limpa_lista(descritor *head);
void pega_flag(int argc, char **argv, char parg[100], char earg[100], int *threads);
void insert2(descritor *head, int in, int go);
int abs(int num);
///Metodos de execução
//void metodo1(char evento[100], char elevar[100], int andar, int threads);
void metodo2(char evento[100],char elevar[100], int threads);

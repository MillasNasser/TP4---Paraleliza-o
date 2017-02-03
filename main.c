/* Programa Simulador de Elevador
 *
 * Millas Násser
 * 05/set/2016
 * */
#include "func.h"

int main(int argc, char **argv){
	int threads;
	char parg[100];
	char earg[100];
	char auxi[500];
	pega_flag(argc,argv,parg,earg,&threads);

	//sprintf(auxi,"./m1 %s %s %d > metodo1.txt",parg,earg,threads);system(auxi);

	struct rusage uso;
	struct timeval inicio,fim;

	getrusage(RUSAGE_SELF,&uso);
	inicio = uso.ru_utime;
		sprintf(auxi,"./m2 %s %s %d",parg,earg,threads);system(auxi);
	getrusage(RUSAGE_SELF,&uso);
	fim = uso.ru_utime;

	FILE *timeM2 = fopen("timeM2.time","a");
	fprintf(timeM2,"%s: %ld\n",parg,(fim.tv_sec*1000+fim.tv_usec)-(inicio.tv_sec*1000+inicio.tv_usec));
	fclose(timeM2);
return 0;
}

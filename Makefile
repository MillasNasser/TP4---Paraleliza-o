########### Makefile ###########
#Variaveis básicas
name=tp1
header=func.h
src1=1-otimizado.c
src2=2-Fifo_simples.c
src3=3-ordenado.c
src4=4-quase_otimizado.c
function=func.c
#Variaveis de dependencias
dpnd1=1-otimizado.o
dpnd2=2-Fifo_simples.o
dpnd3=3-ordenado.o
dpnd4=4-quase_otimizado.o
dpndf=func.o
#
all: $(dpndf)
	gcc $(dpndf) main.c -o $(name) -lpthread
	gcc $(dpndf) m1.c -o m1 -lpthread
	gcc $(dpndf) m2.c -o m2 -lpthread
	rm -rf *.out *.o *.gch
$(dpndf): $(header) $(function)
	gcc -c $(header) $(function)
clear:
	rm -rf *.out *.o *.gch *.txt tp1

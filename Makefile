program: fp_parce.o
	gcc fp_parce.o -g -o program -lm
main.o: main.c
	gcc -c -g fp_parce.c -lm
clean: 
	rm *.o program

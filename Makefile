output: main.o 
	mpirun -n 21 Main
	
main.o: Main.c
	mpicc -o Main Main.c -lm
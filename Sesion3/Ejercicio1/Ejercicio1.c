#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	pid_t pid;

	if (argc != 2){
		perror("\nError, parametro = un número a comprobar");
		exit(-1);
	}

	if((pid = fork()) < 0){
		perror("\nError en el fork");
		exit(-1);
	}

	int numero = argv[1];

	if(pid == 0){	/* Proceso Hijo */
		printf("Hola, soy el proceso hijo y mi pid es %d y el de mi padre es %d,y voy a comprobar si el número introducido es par o impar \n",getpid(),getppid());
		if((numero % 2) == 1)
			printf("El número %d introducido es impar \n",numero);
		else
			printf("El número %d introducido es par \n",numero);
	}
	else if(pid){
		printf("Hola, soy el proceso padre y mi pid es %d y el de mi hijo es %d, y voy a comprobar si el número introducido es divisible por 4 \n",getpid(),pid);
		if(numero % 4 == 0)
		printf("El número %d introducido es divisible por 4 \n",numero);
	else
		printf("El número %d introducido no es divisible por 4 \n",numero);
	}
	return 0;
}

/*
int main(int argc, char *argv[]){
	pid_t pid;
	pid = fork();
	int num = (int) strtol(argv[1],NULL,10);

	if (pid > 0){	// Es el proceso padre
		if(num % 4 == 0)
			printf("El número %d es divisible por 4\n", num);
	}
	else if (pid == 0){	// Es el proceso hijo
		if(num % 2 == 0)
			printf("El número %d es par\n", num);
	}
}
*/

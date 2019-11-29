#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
	int PIDs[5];
  pid_t PID;
  int estado, hijos = 5, i;

  // vaciar el buffer, para que los printf salgan como deben
  if(setvbuf(stdout,NULL,_IONBF,0)){
    perror("\nError en setvbuf");
  }

  printf("Se va a empezar a ejecutar el proceso creador de hijos\n");

  for(int i=1; i<6; i++){
    if((PIDs[i-1] = fork()) < 0){
      perror("\nError en el fork");
      exit(-1);
    }
    else if(PIDs[i-1] == 0){  // proceso hijo ejecutando el programa
      printf("Soy el hijo %d\n",getpid());
      exit(0);
    }
  }

  for(i=0; i<5; i+=2){  // proceso padre ejecutando el programa
    waitpid(PIDs[i],&estado,SIGCHLD);
    printf("Acaba de finalizar mi hijo con PID = %d y estado %d\n",PIDs[i],estado);
    printf("Solo me quedan %d hijos vivos, este es el %d hijo.",--hijos,i+1);
  }

  for(i=1; i<4; i+=2){  // proceso padre ejecutando el programa
    waitpid(PIDs[i],&estado,SIGCHLD);
    printf("Acaba de finalizar mi hijo con PID = %d y estado %d\n",PIDs[i],estado);
    printf("Solo me quedan %d hijos vivos, este es el %d hijo.",--hijos,i+1);
  }
  exit(0);
}

/* Ejercicio1 */
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#define BUFFER_TAMA 80

int main(int argc, char *argv[]){
	int fd_leer;
	int fd_escribir;
	int leidos;
	char buf[BUFFER_TAMA*sizeof(char)];

	char tmpstr[40];

	if (argc==2){
		fd_leer = open(argv[1], O_RDONLY);
	}
	else{
		fd_leer = STDIN_FILENO;
	}

	if ((fd_escribir = open("salida.txt", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR)) < 0){
		printf("\nError %d en open del fichero de salida", errno);
		perror("\nError en open");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	while((leidos = read(fd_leer, buf, 80)) == 80){
		sprintf(tmpstr, "\nBLOQUE%d\n\n", i);
		i++;
		write(fd_escribir, tmpstr, strlen(tmpstr));
		write(fd_escribir, buf, 80);
	}

	sprintf(tmpstr, "\nBLOQUE%d\n\n", i);
	write(fd_escribir, tmpstr, strlen(tmpstr));
	write(fd_escribir, buf, 80);

	if(lseek(fd_escribir, 0, SEEK_SET) < 0){
		perror("\nError en lseek");
		exit(EXIT_FAILURE);
	}
	
	sprintf(tmpstr, "El numero de bloques leidos es %d\n", i);
	write(fd_escribir, tmpstr, strlen(tmpstr));

	close(fd_escribir);
	close(fd_leer);

	return 0;
}

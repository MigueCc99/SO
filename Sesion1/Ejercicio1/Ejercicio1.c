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
	char caracter[1];
	char salto_linea[2] = "\n";
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
	while(read(fd_leer, buf, BUFFER_TAMA) == 80){
		sprintf(tmpstr, "%s%d\n", "BLOQUE", i);
		i++;
		write(fd_escribir, salto_linea, strlen(salto_linea));
		write(fd_escribir, tmpstr, strlen(tmpstr));
		write(fd_escribir, buf, BUFFER_TAMA);
	}

	sprintf(tmpstr, "%s%d\n", "BLOQUE", i);

	memset(buf, 0, BUFFER_TAMA);		// Limpia el buffer

	write(fd_escribir, salto_linea, strlen(salto_linea));
	write(fd_escribir, tmpstr, strlen(tmpstr));

	while(read(fd_leer, caracter, 1) != 0){
		write(fd_escribir, caracter, 1);
	}

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

/*
int main(int argc, char * argv[]){

    int cont = 1;
    int leidos;
    int filein, fileout;
    char cadena[30];
    char bloque[40];
    char salto_linea[2] = "\n";
    char caracter[1];
    int num_char = 1;

    if (argc == 2){
        filein = open(argv[1], O_RDONLY);
    }
    else{
        filein = STDIN_FILENO;
    }

    fileout = open("archivo_salida", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);

    if (fileout < 0){
        printf("El fichero de salida no se pudo abrir de forma correcta\n");
        exit(-1);
    }

    while ((leidos = read(filein, caracter, 1)) != 0){

        if (num_char == 1 || num_char % 80 == 0){

            if (num_char != 1)
                write(fileout, salto_linea, strlen(salto_linea));

            else{
                sprintf(bloque, "El numero de bloques es <%d>\n", cont);
                write(fileout, bloque, strlen(bloque));
            }

            sprintf (bloque, "%s%d\n", "Bloque", cont);
            write(fileout, bloque, strlen(bloque));
            cont++;
        }

        write(fileout, caracter, 1);
        num_char++;
    }

    sprintf(bloque, "El numero de bloques es <%d>\n", cont);
    lseek(fileout, 0, SEEK_SET);
    write(fileout, bloque, strlen(bloque));

    close(filein);
    close(fileout);

    return 0;
}



int main(int argc, char *argv[]){
	int cont = 1, leidos;
	int filein, fileout;
	char cadena[70];
	char cadena_bloque[80];
	char salto_linea[2] = "\n";
	char caracter[1];
	int num_char = 1;

	if (argc == 2){
		filein = open(argv[1], O_RDONLY);
	}else{
		filein = STDIN_FILENO;
	}

	fileout = open("salida.txt", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWURS);

	if (fileout < 0){
		printf("El fichero de salida no se pudo abrir correctamente\n");
		exit(EXIT_FAILURE);
	}

	while((leidos = read(filein, caracter, 1)) != 0){
		if (num_char == 1 || num_char%80 == 0){
			if (num_char != 1)
				write(fileout, salto_linea, strlen(salto_linea));
			else{
				sprintf(cadena_bloque, "El número de bloques es <%d>\n ", cont);
				write(fileout, cadena_bloque, strlen(cadena_bloque));
			}

			sprintf(cadena_bloque, "%s%d\n", "Bloque", cont);
			write(fileout, cadena_bloque, strlen(cadena_bloque));

			cont++;
		}
	}

}
*/



/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 *
 *  Miguel Ángel Campos Cubillas <miguecc99@correo.ugr.es>
 *
 * 	Este programa pide como argumentos un pathname y un numero octal para definir permisos,
 * 	se pide que se cambien los permisos de los archivos conetenidos en el directorio indicado
 * 	y se muestre una lista de los archivos cambiados indicando los permisos antiguos y los nuevos
 */

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>

// PRIMERA FORMA 

int main(int argc, char *argv[]){
	// Declaración de variables
	struct stat atributos;
	struct dirent *direntp;
	DIR *dirp;

	int stat_result, chmod_result, permisos_antiguos, permisos_nuevos;

	// Comprobamos los argumentos (pathname y permisos)
	if(argc != 3){
		printf("Modo de uso: %s <pathname> <permisos>\n", argv[0]);
		exit(1);
	}

	// Open => directorio
	dirp = opendir(argv[1]);

	if(dirp == NULL){	// Si no ha habido éxito al abrir el directorio
		printf("Error: No se puede abrir el directorio : %s\n", argv[1]);
		exit(2);
	}

	// Leemos las entradas del directorio
	printf("<nombre>\t<ant>\t\t<new>\n");

	while((direntp = readdir(dirp)) != NULL){
		// Cambiamos el working directory al que le pasamos
		// aunque también le podríamos haber pasado el full path al stat
		chdir(argv[1]);

		stat_result = stat(direntp->d_name,&atributos);
		if (stat_result < 0){
			printf("Error al intentar acceder a los atributos de %s\n", direntp->d_name);
		}
		else{
			// Comprobamos que no sea un directorio (ni los enlaces . y ..)
			if(!S_ISDIR(atributos.st_mode)){

				// Asignamos los permisos nuevos pasados como parámetro
				permisos_nuevos = strtol(argv[2], 0, 8) & 0777;

				// Guardamos los permisos antiguos
				permisos_antiguos = atributos.st_mode & 0777;

				// Intentamos cambiar los permisos
				chmod_result = chmod(direntp->d_name, permisos_nuevos);

				if(chmod_result < 0){
					// No se han podido cambiar los permisos, usamos %o para que muestre el valor octal
					printf("%s : \t%d \t\t%o\n", direntp->d_name, chmod_result, permisos_antiguos);
					exit(-1);
				}
				else{
					// Se han cambiado los permisos, usamos %o para que se muestre el valor octal
					printf("%s : \t\t%o \t\t%o\n", direntp->d_name, permisos_antiguos, permisos_nuevos);
				}

			}
		}
	}

	// Cerramos el directorio
	closedir(dirp);

	return 0;

}


/*
#include<sys/types.h> 
#include<sys/stat.h>
#include<fcntl.h> 
#include<string.h> 
#include<stdlib.h> 
#include<stdio.h>
#include<errno.h> 
#include<unistd.h> 
#include<dirent.h>

int main(int argc, char *argv[]){
	DIR *direct;
	unsigned int permisos;
	char *pathname;
	struct stat atributos;
	struct dirent *ed;
	char cadena[100];
	char cadena2[100];
	extern int errno;

	if(argc == 3){
		pathname = argv[1];
		direct = opendir(pathname);
		permisos = strtol(argv[2],NULL,8);
	}
	else{
		printf("Uso: ./Ejercicio1 <pathname> <permisos>\n");
		exit(-1);
	}

	readdir(direct);

	while((ed = readdir(direct)) != NULL){
		sprintf(cadena,"%s/%s",pathname,ed->d_name);

		if(stat(cadena,&atributos) < 0){
			printf("Error al intentar acceder a los atributos del archivo");
			perror("\nError en lstat");
			exit(-1);
		}

		if(S_ISREG(atributos.st_mode)){
			sprintf(cadena2,"%s",ed->d_name);

			printf("%s: %o",cadena2,atributos.st_mode);

			chmod(cadena,permisos);

			if(chmod(cadena,permisos) < 0)
				printf("Error: %s\n",strerror(errno));
			else{
				stat(cadena,&atributos);
				printf("%o \n",atributos.st_mode);
			}
		}
	}
	closedir(direct);
	return 0;
}
*/


/* SEGUNDA FORMA */
/*
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>

int main(int argc, char * argv[])
{

	DIR * direct;
	unsigned int permisos;
	char * pathname;
	struct dirent * puntero;
	char cadena[100];
	char cadena2[400];
	struct stat atributos;

	if (argc != 3){
		printf("ERROR, se deben introducir %d argumentos\n", 2);
		exit(1);
	}

	pathname = argv[1];
	permisos = strtol(argv[2], NULL, 8);
	direct = opendir(pathname);

	if (direct == NULL){
		printf("ERROR, no se ha podido abrir el directorio\n");
		exit(2);
	}

	readdir(direct); // Abrimos la lectura del directorio
	puntero = readdir(direct); // Leemos el archivo /. del directorio

	while ((puntero = readdir(direct)) != NULL) {
		sprintf(cadena, "%s%s", pathname, puntero->d_name);

		if(stat(cadena, &atributos) < 0){
			printf("ERROR, no se ha podido acceder a los metadatos de ");
			printf(cadena);
			printf("\n");
			exit(3);
		}

		sprintf(cadena2, "%s", puntero->d_name);
		
		if (chmod(cadena, permisos) < 0){
			printf("ERROR, no se ha podido realizar el cambio de permisos para el archivo ");
			printf("%s: %d %o", cadena2, errno, atributos.st_mode);
			printf("\n");
		}

		else {
			stat(cadena, &atributos);
			printf("%s: %o ", cadena2, atributos.st_mode);
			printf("%o \n", atributos.st_mode);
		}
	}
		
	closedir(direct);
}
*/








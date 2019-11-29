/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática

 *
 *  Miguel Ángel Campos Cubillas <miguecc99@correo.ugr.es>
 *
 * Programa una nueva orden que recorra la jerarquia de subdirectorios existentes a partir 
 * de uno dado como argumento y devuelva la cuenta de todos aquellos archivos regulares que 
 * tengan permiso de ejecucion para el grupo y para otros. Ademas del nombre de los archivos 
 * encontrados, debera devolver sus numeros de inodo y la suma total de espacio ocupado por 
 * dichos archivos. 
 */

/* PRIMERA VERSIÓN */
/*
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <ftw.h>

int n_files = 0;
int size = 0;

int visitar(const char *path, const struct stat *stat, int flags, struct FTW *ftw){
	// Comprobamos que los archivos sean regulares y sean ejecutables para grupo y otros
	if((S_ISREG(stat->st_mode) && (stat->st_mode & S_IXGRP) && (stat->st_mode & S_IXOTH))){
		// Los inodos son unsigned long long, así que para que los pinte sprintf hay que usar llu
		printf("%s %llu\n",path,stat->st_ino);

		// Incrementos de las variables globales
		size += stat->st_size;
		n_files++;
	}
	return 0;
}


int main(int argc, char *argv[]){

	printf("Los i-nodos son:\n");

	// Llamamos a nftw pasandole como parámetro la función visitar
	// Si le hemos pasado una ruta como parámetro la usará en lugar del directorio actual (.)
	if(nftw(argc >= 2 ? argv[1] : ".", visitar, 10, 0) != 0)
		perror("nftw");

	printf("Existen %d archivos regulares con permiso x para grupo y otros\n", n_files);
	printf("El tamaño total ocupado por dichos archivos es %d bytes\n", size);

	return 0;

}
*/

/* SEGUNDA VERSIÓN */
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<dirent.h>
#define mymask(mode) ((mode) & ~S_IFMT)
// Permisos de ejecución para grupo y otros
#define S_IFXGRPOTH 011
// Se define la macro con la regla para comprobar si tiene permiso x en grupo y otros.
#define regla1(mode) ((((mode) & ~S_IFMT) & 011) == S_IFXGRPOTH)

void buscar_dir(DIR *direct, char pathname[], int *reg, int *tamanio){
	struct stat atributos;
	struct dirent *ed;
	DIR *direct_act;
	char cadena[500];

	while((ed=readdir(direct)) != NULL){
		// Ignorar el directorio actual y el superior
		if(strcmp(ed->d_name,".") != 0 && strcmp(ed->d_name,"..") != 0){
			sprintf(cadena,"%s/%s",pathname,ed->d_name);

			if(stat(cadena,&atributos) < 0){
				printf("\nError al intentar acceder a los metadatos del archivo");
				perror("\nError en lstat");
				exit(-1);
			}

			if(S_ISDIR(atributos.st_mode)){
				if((direct_act = opendir(cadena)) == NULL)
					printf("\nError al abrir el directorio: [%s]\n",cadena);
				else
					buscar_dir(direct_act,cadena,reg,tamanio);
			}
			else{
				printf("%s %llu \n",cadena,atributos.st_ino);
				if(S_ISREG(atributos.st_mode)){
					if(regla1(atributos.st_mode)){
						(*reg)++;
						(*tamanio) += (int)atributos.st_size; 
					}
				}
			}
		}
	}	
	closedir(direct);
}

int main(int argc, char *argv[]){
	DIR *direct;
	char pathname[500];
	int reg, tamanio = 0;

	if(argc == 2){
		strcpy(pathname,argv[1]);
	}
	else{
		strcpy(pathname,".");
	}


	if((direct=opendir(pathname)) == NULL){
		printf("\nError al abrir directorio\n");
		exit(-1);
	}

	printf("Los inodos son: \n\n");

	buscar_dir(direct,pathname,&reg,&tamanio);

	printf("Hay %d archivos regulares con x permiso para grupo y otros\n",reg);
	printf("El tamaño total ocupado por dichos archivos es %d bytes\n",tamanio);

	return 0;
}






/* TERCERA VERSIÓN */
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

void buscar_dir (char * pathname, int *num_archivos, int *tamanio){
	DIR *direct;
	struct dirent *pt;
	struct stat atributos;
	char cadena[100];

	direct = opendir(pathname);

	if (direct == NULL){
		printf("ERROR, no se pudo abrir el directorio %s \n", pathname);
		exit(2);
	}

	while ((pt = readdir(direct)) != NULL){
		// Ignorar el directorio actual y superior
		if (strcmp(pt->d_name, ".") != 0 && strcmp(pt->d_name, "..") != 0){
			sprintf(cadena, "%s/%s", pathname, pt->d_name);
			if(stat(cadena, &atributos) <0){
				printf ("ERROR, no se pudo acceder a los metadatos del archivo ");
				printf("%s",cadena);
				printf("\n");	
				exit(3);
			}

			if (S_ISDIR(atributos.st_mode))
				buscar_dir(cadena, num_archivos, tamanio);
			else{
				printf("%s %ld \n", cadena, atributos.st_ino);
				(*tamanio) += atributos.st_size;
				if(S_ISREG(atributos.st_mode) && ((atributos.st_mode & 001) == S_IXOTH) && ((atributos.st_mode & 01) == S_IXGRP))
					(*num_archivos)++;
			}
		}
	}
}

int main (int argc, char * argv[]){
	char pathname[100];
	int num_archivos = 0;
	int tam = 0;

	if (argc == 2)
		strcpy(pathname, argv[1]);
	else
		strcpy(pathname, ".");

	printf("Los i-nodos son: \n");

	buscar_dir(pathname, &num_archivos, &tam);

	printf("Hay %o archivos regulares con permiso de ejecución para grupo y otros \n", num_archivos);
	printf("El tamaño total ocupado por dichos archivos es %o bytes\n", tam);
}

*/













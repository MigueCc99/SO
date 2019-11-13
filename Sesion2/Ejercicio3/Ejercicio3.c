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
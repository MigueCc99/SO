
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
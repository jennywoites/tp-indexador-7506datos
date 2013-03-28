#include "parserIndex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


//Para poder ver lo que haya en un directorio:
#include <dirent.h>


int parserIndex_obtenerParametros(int argc, char** argv,char** cadenas){
	if (argc != 3){
		//ver si se hace algo en caso de tener argumentos de mas o de menos
		return PARSERINDEX_ERROR;
	}

	cadenas[0] = malloc (sizeof(char) * (strlen(argv[1])+1));
	cadenas[1] = malloc (sizeof(char) * (strlen(argv[2])+1));

	strcpy(cadenas[0], argv[1]);
	strcpy(cadenas[1], argv[2]);

	return PARSERINDEX_OK;
}

int filtro (struct dirent* d){
	if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) return 0;

	return 1;
}

int parserIndex_obtenerRutasDirectorios(char* directorio, char*** rutas, int* cant){
	struct dirent** directorios = NULL;


	(*cant) = scandir(directorio, &directorios, filtro, NULL);
	//El tercer campo es una funcion de filtro que si devuelve 0, hara que se ignore
	//el directorio/archivo leido. El cuarto campo es una funcion de ordenamiento
	//(creo que se puede mandar una que se llama alphasort());

	char** r = malloc (sizeof(char*) * (*cant));
	if (!rutas) return PARSERINDEX_ERROR;

	for (int i = 0; i < (*cant); i++){
		r[i] = malloc (sizeof(char)*(strlen(directorios[i]->d_name) + strlen(directorio)+2));
		sprintf(r[i], "%s/%s", directorio, directorios[i]->d_name);
		free(directorios[i]);
	}
	free(directorios);
	*rutas = r;
	return PARSERINDEX_OK;
}

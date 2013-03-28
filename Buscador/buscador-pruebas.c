#include <stdio.h>
#include "Parseo/parserFinder.h"
#include <stdlib.h>

int main (int argc, char** argv){
	char* cadenas[2];

	if (parserFinder_obtenerParametros(argc, argv, cadenas) == PARSERFINDER_OK){
		printf("El parseo se realizo correctamente. Repositorio: %s Query: %s\n", cadenas[0], cadenas[1]);
		free(cadenas[0]);
		free(cadenas[1]);
	}else{
		printf("Hubo un error al parsear la busqueda\n");
	}

	return 0;
}

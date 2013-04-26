#include <stdio.h>
#include "Indexador/Parseo/parserIndex.h"
#include <stdlib.h>
#include <stdbool.h>

const char* SALIDA_PARSER = "parser.txt";

void print_test(char* name, bool result){
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}


int main (int argc, char** argv){

	char** rutas;
	unsigned long cant;

	const char* directorio = "ejs_parser";

	int aux = parserIndex_obtenerRutasDirectorios(directorio, &rutas, &cant);

	if (aux == PARSERINDEX_ERROR){
		printf ("Error al obtener los archivos\n");
		return 0;
	}

	aux = parserIndex_parsearArchivos(rutas,cant,SALIDA_PARSER);

	if (aux == PARSERINDEX_ERROR)
		printf("Error al parsear\n");

	else
		printf("Todo ok\n");

	for (unsigned long i = 0; i < cant; i++){
		free(rutas[i]);
	}
	free(rutas);
	return 0;
}

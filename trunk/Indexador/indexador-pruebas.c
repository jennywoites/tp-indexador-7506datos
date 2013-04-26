#include <stdio.h>
#include "Indexador/Parseo/parserIndex.h"
//#include "Indexador/Sort/sorting.h"
#include <stdlib.h>
#include <stdbool.h>

const char* SALIDA_PARSER = "parser.jem";
const char* SALIDA_SORT = "sort.jem";
const char* INDICE = "indice.jem";
const char* LEXICO = "lexico.jem";
const char* NOMBRE_ARCHIVOS = "archs.jem";


void print_test(char* name, bool result){
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}


int main (int argc, char** argv){

	char** rutas;
	unsigned long cant;

	const char* directorio = "ejs_parser";

	int aux = parserIndex_obtenerRutasDirectorios(directorio, &rutas, &cant);

	if (aux != PARSERINDEX_OK){
		printf ("Error al obtener los archivos\n");
		return 0;
	}

	aux = parserIndex_parsearArchivos(directorio, rutas,cant,SALIDA_PARSER, NOMBRE_ARCHIVOS);

	if (aux == PARSERINDEX_OK){
		/* aux = sorting_ordenarArchivo(SALIDA_PARSER, SALIDA_SORT);
		 * if (aux != SORTING_OK){ printf("hubo un error al ordenar los terminos\n");}
		 * else{
		 *		indexar(SALIDA_SORT, DATOS, LEXICO);
		 * }
		*/
		remove(SALIDA_PARSER);
		remove(SALIDA_SORT);
	}


	for (unsigned long i = 0; i < cant; i++){
		free(rutas[i]);
	}
	free(rutas);
	return 0;
}

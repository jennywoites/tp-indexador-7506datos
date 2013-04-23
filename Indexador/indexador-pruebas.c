#include <stdio.h>
#include "Parseo/parserIndex.h"
#include "Manejo de Archivos/merger.h"
#include "Carpetas Compartidas/Codigos/codigo.h"
#include <stdlib.h>
#include <stdbool.h>

void print_test(char* name, bool result){
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}


int main (int argc, char** argv){

	char* cadenas[2];
	if (parserIndex_obtenerParametros(argc, argv, cadenas) == 0){
		printf("Se parseo bien. Nombre de repo: %s, Directorio: %s\n", cadenas[0], cadenas[1]);

		char** dirs = NULL;
		int cant;



		if (parserIndex_obtenerRutasDirectorios(cadenas[1], &dirs, &cant) == PARSERINDEX_OK){
			printf("Se encontraron los siguientes archivos:\n");
			for (int i = 0; i < cant; i++){
				printf("%s\n",dirs[i]);
			}
		}else{
			printf("Hubo algun error en la busqueda por directorios\n");
			return 1;
		}

		free(cadenas[0]);
		free(cadenas[1]);
		if (merger_MergearArchivos(dirs, cant) == MERGER_ERROR) printf("Se cago la cosa\n");


		for (unsigned long j = 0; j < cant; j++){
			if (parserIndex_parsearArchivo(dirs[j],j+1) == PARSERINDEX_OK) printf("OK \n");
			free(dirs[j]);
		}
		free(dirs);
	}else{
		printf("Hubo un error en el parseo\n");
	}


/*	codigo_t* code;
	code = codigo_crearBinario(1,7);
	codigo_imprimir(code);
	codigo_destruir(code);*/

	return 0;
}

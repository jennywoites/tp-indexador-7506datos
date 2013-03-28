#include <stdio.h>
#include "Parseo/parserIndex.h"
#include <stdlib.h>

int main (int argc, char** argv){

	char* cadenas[2];
	if (parserIndexObtenerParametros(argc, argv, cadenas) == 0){
		printf("Se parseo bien. Nombre de repo: %s, Directorio: %s\n", cadenas[0], cadenas[1]);
		free(cadenas[0]);
		free(cadenas[1]);
	}else{
		printf("Hubo un error en el parseo");
	}

	return 0;
}

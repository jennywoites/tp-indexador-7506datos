#include "../funcionesGeneralesArchivos.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int mainF (){
	char * directorio = malloc(sizeof(char)*10);
	strcpy(directorio, "hola");
	
	char* resul = __crear_ruta_repo(directorio, "parser.jem");
	
	printf("this: %s \n", resul );
	
	free(directorio);
	free(resul);
	
	return 0;
}

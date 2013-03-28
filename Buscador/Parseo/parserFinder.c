#include "parserFinder.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

const char* REPO_CHICO = "-r";
const char* REPO_GRANDE = "--repository";
const char* QUERY_CHICO = "-q";
const char* QUERY_GRANDE = "--query";

int parserFinder_obtenerParametros(int argc, char** argv,char** cadenas){
	/*Tienen que venir 5 parametros:
	 * 1) El que siempre viene (el nombre del programa)
	 * 2) y 3) -q/--query y -r/--repository
	 * 4) y 5) El nombre del repositorio y la busqueda a realizar, entre comillas.
	 * Se debe seguir el siguiente formato:
	 * ./programa -r Name -q "query"
	 * Estando los parametros posibles para alternarse libremente, mientras se siga manteniendo el formato
	 */

	if (argc != 5){
		//ver si decimos que se hace algo cuando vienen parametros de mas o de menos
		return PARSERFINDER_ERROR;
	}

	//Busco el parametro de repositorio
	if (strcmp(argv[1], REPO_CHICO) == 0 || strcmp(argv[1], REPO_GRANDE) == 0 ){
		cadenas[0] = malloc (sizeof(char) * (strlen(argv[2]) + 1));
		strcpy(cadenas[0], argv[2]);
	}else if (strcmp(argv[3], REPO_CHICO) == 0 || strcmp(argv[3], REPO_GRANDE) == 0 ){
		cadenas[0] = malloc (sizeof(char) * (strlen(argv[4]) + 1));
		strcpy(cadenas[0], argv[4]);
	}else{
		//No encuentra el parametro de repositorio

		return PARSERFINDER_ERROR;
	}

	//Busco el parametro de querys
	if (strcmp(argv[1], QUERY_CHICO) == 0 || strcmp(argv[1], QUERY_GRANDE) == 0 ){
		cadenas[1] = malloc (sizeof(char) * (strlen(argv[2]) + 1));
		strcpy(cadenas[1], argv[2]);
	}else if (strcmp(argv[3], QUERY_CHICO) == 0 || strcmp(argv[3], QUERY_GRANDE) == 0 ){
		cadenas[1] = malloc (sizeof(char) * (strlen(argv[4]) + 1));
		strcpy(cadenas[1], argv[4]);
	}else{
		//No encuentra el parametro de querys
		free(cadenas[0]);
		return PARSERFINDER_ERROR;
	}
	return PARSERFINDER_OK;
}

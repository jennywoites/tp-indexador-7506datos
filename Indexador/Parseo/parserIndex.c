#include "parserIndex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../Manejo de Archivos/funcionesGeneralesArchivos.h"

//Para poder ver lo que haya en un directorio:
#include <dirent.h>
#define DT_DIR 4
#define DT_FILE 8

#define TAM 50

const char SEPARADORES[] = {' ', '-', '\n', '/'};
unsigned int CANT_SEPARADORES = 4;
const char PRESCINDIBLES[] = {',' , ';' , '.' , '?' , '!'};
unsigned int CANT_PRESCINDIBLES = 5;
const char DUPLICANTES[] = {'\"'};
unsigned int CANT_DUPLICANTES = 1;

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

	//Por ahora, a los sub-directorios los descartamos (y cualquier cosa que no sea un archivo)
	if (d->d_type != DT_FILE) return 0;
	return 1;
}

int parserIndex_obtenerRutasDirectorios(char* directorio, char*** rutas, int* cant){
	struct dirent** directorios = NULL;


	(*cant) = scandir(directorio, &directorios, filtro, NULL);
	//El tercer campo es una funcion de filtro que si devuelve 0, hara que se ignore
	//el directorio/archivo leido. El cuarto campo es una funcion de ordenamiento
	//(creo que se puede mandar una que se llama alphasort());

	if ((*cant) <= 0) return PARSERINDEX_ERROR;
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

/* **************************************************************************** */

char* eliminarCaracteresPrescindibles(char*, bool);
void tratarPalabra(char*, const char*, unsigned int);
bool caracterDeSeparacion(char c);
bool esNecesarioDuplicar(char*);
void __toLowerCase(char*);

int parserIndex_parsearArchivo(const char* ruta_archivo){
	FILE* arch = fopen(ruta_archivo, lectura_archivos());
	if (!arch) return PARSERINDEX_ERROR;

	unsigned int i;
	unsigned int tam = TAM;
	unsigned int pos = 0;
	while (!feof(arch)){
		char* buffer = malloc (sizeof(char) * TAM);
		i = 0;
		char c;
		while (lectura_anticipada(arch, &c) && !caracterDeSeparacion(c)){
			if (i == tam){
				tam += TAM;
				buffer = realloc (buffer, sizeof(char)*tam);
			}
			buffer[i] = c;
			i++;
		}
		if (i == tam)
			buffer = realloc (buffer, sizeof(char)*(tam+1));
		buffer[i] = '\0';

		__toLowerCase(buffer);

		char* bufferSinEliminables = eliminarCaracteresPrescindibles(buffer, false);

		tratarPalabra(bufferSinEliminables, ruta_archivo, pos);

		if (esNecesarioDuplicar(bufferSinEliminables)){
			char* bufferSinDuplicantes = eliminarCaracteresPrescindibles(buffer, true);
			tratarPalabra(bufferSinDuplicantes, ruta_archivo, pos);
			free(bufferSinDuplicantes);
		}

		free(buffer);
		free(bufferSinEliminables);
		pos++;
	}

	fclose(arch);
	return PARSERINDEX_OK;
}

bool caracterDeSeparacion(char c){

	for (unsigned int i = 0; i < CANT_SEPARADORES; i++)
		if (c == SEPARADORES[i]) return true;

	return false;
}

bool caracterDuplicante(char c){
	for (unsigned int i = 0; i < CANT_DUPLICANTES; i++){
		if (c == DUPLICANTES[i]) return true;
	}
	return false;
}

char* eliminarCaracteresPrescindibles(char* cadena, bool duplicante){
	char* nueva = malloc (sizeof(char)* (strlen(cadena)+1));
	unsigned int cant = 0;
	for (unsigned int i = 0; i < strlen(cadena); i++){
		bool elim = false;
		unsigned int j = 0;
		while (!elim && j < CANT_PRESCINDIBLES){
			elim = (cadena[i] == PRESCINDIBLES[j]);
			j++;
		}

		if(!elim){
			if (!duplicante || !caracterDuplicante(cadena[i])){
				nueva[cant] = cadena[i];
				cant++;
			}
		}
	}
	nueva[cant] = '\0';
	return nueva;
}

bool esNecesarioDuplicar(char* cadena){
	//Por ahora la hago simple:
	for (unsigned int i = 0 ; i < CANT_DUPLICANTES; i++){
		if (cadena[0] == DUPLICANTES[i] || cadena[strlen(cadena)-1] == DUPLICANTES[i])
			return true;
	}
	return false;
}

void tratarPalabra(char* palabra, const char* texto, unsigned int pos){
	//Por ahora voy a imprimir simplemente:
	printf("%s\t\t%s\t%u\n", palabra, texto, pos);
}

void __toLowerCase(char* cadena){
	for (unsigned int i = 0; i < strlen(cadena); i++){
		char c = cadena[i];
		if (c <= 'Z' && c >= 'A'){
			cadena[i] = c - ('A' - 'a');
		}
	}
}

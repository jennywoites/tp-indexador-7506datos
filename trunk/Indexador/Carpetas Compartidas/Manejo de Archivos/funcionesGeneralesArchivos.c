#include "funcionesGeneralesArchivos.h"
#include <stdlib.h>
#include <stddef.h>
#define MAXIMO_LINEAS 300
#define MAX_LONG_LINEA 50

const char* LECTURA = "r";
const char* ESCRITURA = "w";
bool lectura_anticipada(FILE* arch, char* c){
	if (feof(arch)) return false;
	(*c) = fgetc (arch);
	if ((*c)==EOF) return false;
	return true;
}


const char* lectura_archivos(){
	return LECTURA;
}
const char* escritura_archivos(){
	return ESCRITURA;
}

char** obtenerLineas(FILE* archivo, unsigned int* cant){
	char** lineas = malloc (sizeof(char*) * MAXIMO_LINEAS);

	char c;
	unsigned int i, j = 0;

	while (!feof(archivo)){
		char* buffer = malloc (sizeof(char) * MAX_LONG_LINEA);
		i = 0;
		do{
			c = fgetc(archivo);
			if (c == '\n')
				c = '\0';
			buffer[i] = c;
			i++;
		}while (!feof(archivo) && c != '\0' && c != '\n');

		if (feof(archivo)){free(buffer); continue;}

		lineas[j] = buffer;
		j++;
	}

	*cant = j;
	return lineas;
}

void verificarYAumentarTam(char** buffer, unsigned int* tam, unsigned int pos){
	if ((*tam) != pos) return;

	(*tam) = (*tam) + MAX_LONG_LINEA;

	char* aux = realloc (*buffer, sizeof(char) * (MAX_LONG_LINEA));
	if (!aux) return;

	*buffer = aux;
}


char* obtenerLinea(FILE* archivo){
	if (feof(archivo)) return NULL;

	unsigned int tam = MAX_LONG_LINEA;
	char* buffer = malloc (sizeof(char) * tam);
	unsigned int i = 0;
	char c = fgetc(archivo);

	while (c != '\n' && c != EOF){
		verificarYAumentarTam(&buffer, &tam, i);

		buffer[i] = c;
		i++;
		c = fgetc(archivo);
	}

	verificarYAumentarTam(&buffer, &tam, i);
	buffer[i] = '\0';
	return buffer;
}

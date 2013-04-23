#include "registro.h"
#include <stdlib.h>
#include "funcionesGeneralesArchivos.h"
#include <string.h>

struct registro{
	char* termino;
	unsigned long posicion;
	unsigned long documento;
};

int comparacionRegistros(const void* a, const void* b){
	dato_t* d1 = (dato_t*) a;
	dato_t* d2 = (dato_t*) b;

	//Si son terminos distintos ordeno alfanumericamente
	if (strcmp(d2->registro->termino, d1->registro->termino) != 0)
		return strcmp(d2->registro->termino, d1->registro->termino);

	//Mismo termino:
	if (d2->registro->documento != d1->registro->documento)
		return d2->registro->documento - d1->registro->documento;

	return d2->registro->posicion - d1->registro->posicion;
}

registro_t* registro_crear(const char* termino, unsigned long doc, unsigned long pos ){
	registro_t* reg = malloc (sizeof(registro_t));
	if (!reg) return NULL;

	reg->termino = malloc (sizeof(char) * (strlen(termino)+1));
	strcpy(reg->termino, termino);

	reg->documento = doc;
	reg->posicion = pos;
	return reg;
}

unsigned long stringToLong(char*);

registro_t* registro_leer(FILE* entrada){
	char* linea = obtenerLinea(entrada);
	if (!linea) return NULL;

	if (strlen(linea) < 2) {
		free(linea);
		return NULL;
	}

	char* termino = strtok(linea, ";");
	char* doc = strtok (NULL, ";");
	char* pos = strtok(NULL, ";");

	unsigned long documento = stringToLong(doc);
	unsigned long posicion = stringToLong(pos);

	registro_t* reg = registro_crear(termino, documento, posicion);
	free(linea);
	return reg;
}

void registro_escribir(FILE* salida, registro_t* reg){
	if (!reg) return;
	fprintf(salida, "%s;%lu;%lu\n",reg->termino, reg->documento, reg->posicion);
	//printf("TERMINO %s DOCUMENTO %lu POSICION %lu\n", reg->termino, reg->documento, reg->posicion);
}

void registro_destruir(registro_t* reg){
	if (!reg) return;
	free (reg->termino);
	free(reg);
}

unsigned long stringToLong(char* num){
	unsigned long n = 0;
	unsigned mult = 1;

	for ( int i = strlen(num) - 1; i >= 0 ; i--){
		char c = num[i];
		unsigned int digito = c - '0';
		n += digito * mult;
		mult *= 10;
	}
	return n;
}

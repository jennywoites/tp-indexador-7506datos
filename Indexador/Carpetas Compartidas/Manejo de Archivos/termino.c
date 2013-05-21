#include "termino.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"

struct termino{
	char* termino;
	size_t offset;
	size_t frecuencia;
};

termino_t* termino_crear (const char* term, size_t offset, size_t frec){
	if (!term) return NULL;

	termino_t* t = malloc (sizeof(termino_t));
	if (!t) return NULL;

	t->termino = malloc (sizeof(char) * (strlen(term)+1));
	if (!t->termino){
		free(t);
		return NULL;
	}

	strcpy(t->termino, term);
	t->offset = offset;
	t->frecuencia = frec;
	return t;
}

void termino_destruir(termino_t* termino){
	if (!termino) return;

	free(termino->termino);
	free(termino);
}

char* termino_obtenerPalabra(termino_t* term){
	if (!term) return NULL;

	char* copy = malloc (sizeof(char) * (strlen(term->termino) + 1));
	if (!copy) return NULL;
	strcpy(copy, term->termino);
	return copy;
}

termino_t* termino_leer(termino_t* termino_anterior, FILE* archFrontCoding, FILE* archDiferentes){
	char* linea = obtenerLinea(archFrontCoding);
	char* repetidos = strtok(linea, ";");
	char* distintos = strtok(NULL, ";");
	char* offset = strtok(NULL, ";");
	char* frecuencia = strtok(NULL, ";");

	if (repetidos == NULL || distintos == NULL || offset == NULL || frecuencia == NULL){
		free(linea);
		return NULL;
	}

	size_t rep = atoi(repetidos);
	size_t dist = atoi(distintos);
	size_t off = atoi(offset);
	size_t frec = atoi (frecuencia);

	free(linea);

	char* cad = malloc (sizeof(char) * (rep + dist + 1));
	size_t i;
	for (i = 0; i < rep && termino_anterior; i++)
		cad[i] = termino_anterior->termino[i];

	for (i = 0; i < dist; i++)
		cad[i + rep] = fgetc(archDiferentes);
	cad[rep + dist] = '\0';

	size_t off_ant = 0;
	if (termino_anterior)
		off_ant = termino_anterior->offset;

	termino_t* t = termino_crear(cad, off + off_ant, frec);
	free(cad);

	return t;

}

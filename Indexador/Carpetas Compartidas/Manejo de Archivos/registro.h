#ifndef REGISTRO_H_
#define REGISTRO_H_
#include <stdio.h>
#include "../TDAs/lista.h"

typedef struct registro registro_t;

//Estructura auxiliar para poner dentro del heap
typedef struct dato{
	registro_t* registro;
	unsigned int numArchivo;
}dato_t;

/* Funcion que permite crear un registro a partir de un termino, documento y posicion */
registro_t* registro_crear(const char*, unsigned long, unsigned long);

/* Funcion que permite liberar la memoria alocada para el registro */
void registro_destruir(registro_t*);

/* Funcion para compara dos registros */
int comparacionRegistros(const void* a, const void* b);

/* Permite obtener un registro a partir de un archivo*/
registro_t* registro_leer(FILE*);

/* Escribe un registro dentro de un archivo*/
void registro_escribir(FILE*, registro_t*);


void registro_escribirEnIndice(registro_t* actual, registro_t* anterior, FILE* archIndice, FILE* archLexico, lista_t* documentos, lista_t* posiciones_x_documento, unsigned long* offset);

#endif /* REGISTRO_H_ */

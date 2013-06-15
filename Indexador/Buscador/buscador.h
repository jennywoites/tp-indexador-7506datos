#ifndef BUSCADOR_H_
#define BUSCADOR_H_
#include "../Carpetas Compartidas/TDAs/lista.h"
#include "resultado.h"

typedef struct buscador buscador_t;

/* Permite crear un buscador a partir de las rutas de los archivos de Front
 * Coding y otro con los caracteres diferentes.
 * Memoria: Almacena memoria para el buscador*/
buscador_t* buscador_crear(const char* rutaFrontCoding, const char* rutaDiferentes, size_t cant);

/* Permite liberar la memoria reservada para un buscador*/
void buscador_destruir(buscador_t*);

//FIXME: esto deberia devolver un solucion_t*, por ahora solo va a imprimir los OK :P
resultado_t* buscador_buscar(buscador_t* buscador, lista_t* terminos_buscados, const char* dirOffsets, const char* ruta_tams);

void buscador_busquedaPuntual(buscador_t* buscador, const char* termino, const char* index, const char* paths, const char* offsetPaths, const char* ruta_tams);

void buscador_busquedaImperfecta(buscador_t* buscador, lista_t* query, const char* index, const char* paths, const char* offsetPaths, const char* ruta_tams);

#endif /* BUSCADOR_H_ */

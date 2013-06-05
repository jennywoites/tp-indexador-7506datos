#ifndef BUSCADOR_H_
#define BUSCADOR_H_
#include "../Carpetas Compartidas/TDAs/lista.h"


typedef struct buscador buscador_t;

typedef struct resultado resultado_t;

/* Permite crear un buscador a partir de las rutas de los archivos de Front
 * Coding y otro con los caracteres diferentes.
 * Memoria: Almacena memoria para el buscador*/
buscador_t* buscador_crear(const char* rutaFrontCoding, const char* rutaDiferentes);

/* Permite liberar la memoria reservada para un buscador*/
void buscador_destruir(buscador_t*);

//FIXME: esto deberia devolver un solucion_t*, por ahora solo va a imprimir los OK :P
resultado_t* buscador_buscar(buscador_t* buscador, lista_t* terminos_buscados, const char* dirOffsets);

void resultado_destruir(resultado_t* resul);

void resultado_emitirListado(resultado_t* resultado, lista_t* query, const char* paths);

#endif /* BUSCADOR_H_ */

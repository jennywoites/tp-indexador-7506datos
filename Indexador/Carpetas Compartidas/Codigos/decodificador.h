#ifndef DECODIFICADOR_H_
#define DECODIFICADOR_H_

#include <stdlib.h>
#include "../Manejo de Archivos/debuffer.h"
#include "../TDAs/arbolHuff.h"

#define NO_NUMERO 0


typedef struct decodificador decodificador_t;

decodificador_t* decodificador_crear(debuffer_t* debuffer);
void decodificador_destruir(decodificador_t* decodificador);

unsigned int decodificador_decodificarUnario(debuffer_t* decodificador);
unsigned int decodificador_decodificarBinario(debuffer_t* decodificador, size_t longitud);
unsigned int decodificador_decodificarGamma(debuffer_t* decodificador);
unsigned int decodificador_decodificarDelta(debuffer_t* decodificador);
unsigned int decodificador_decodificarGolomb(debuffer_t* debuffer, size_t b, arbol_huff_t* arbol);

#endif /*DECODIFICADOR_H_*/

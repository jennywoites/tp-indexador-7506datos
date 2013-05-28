#ifndef DECODIFICADOR_H_
#define DECODIFICADOR_H_

#include <stdlib.h>
#include "../Manejo de Archivos/debuffer.h"

#define NO_NUMERO 0


typedef struct decodificador decodificador_t;

decodificador_t* decodificador_crear(debuffer_t* debuffer);
void decodificador_destruir(decodificador_t* decodificador);

//unsigned int decodificador_decodificarUnario(decodificador_t* decodificador);
//unsigned int decodificador_decodificarBinario(decodificador_t* decodificador, size_t longitud);
//unsigned int decodificador_decodificarGamma(decodificador_t* decodificador);
//unsigned int decodificador_decodificarDelta(decodificador_t* decodificador);

unsigned int decodificador_decodificarUnario(debuffer_t* decodificador);
unsigned int decodificador_decodificarBinario(debuffer_t* decodificador, size_t longitud);
unsigned int decodificador_decodificarGamma(debuffer_t* decodificador);
unsigned int decodificador_decodificarDelta(debuffer_t* decodificador);

#endif /*DECODIFICADOR_H_*/

#ifndef CODIFICADOR_H_
#define CODIFICADOR_H_

#include <stdlib.h>
#include "../Manejo de Archivos/buffer.h"

#define CODIGO_UNARIO 0
#define CODIGO_BINARIO 1
#define CODIGO_GAMMA 2
#define CODIGO_DELTA 3


typedef struct codificador codificador_t;

codificador_t* codificador_crear(buffer_t* buffer);

void codificador_destruir(codificador_t* cod);

void codificador_codificarUnario(buffer_t* cod, unsigned int num);

void codificador_codificarBinario(buffer_t* cod, unsigned int num, size_t longitud);

void codificador_codificarUnario(buffer_t* cod, unsigned int num);

void codificador_codificarGamma(buffer_t* cod, unsigned int num);

void codificador_codificarDelta(buffer_t* cod, unsigned int num);

void codificador_codificarBinarioPrefijo(buffer_t* cod, unsigned int num, size_t b);

void codificador_codificarGolomb(buffer_t* buffer, unsigned int num, unsigned int b);

#endif /*CODIFICADOR_H_*/

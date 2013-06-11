#ifndef TRASBORDOCODIGO_H_
#define TRASBORDOCODIGO_H_
#include "../Manejo de Archivos/buffer.h"
#include "../Manejo de Archivos/debuffer.h"
#include "codificador.h"
#include "decodificador.h"

/* COMPRESION */
void comprimir_FrecuenciaDocumentos(buffer_t* cod, unsigned int num);

void comprimir_FrecuenciaPosiciones(buffer_t* cod, unsigned int num);

void comprimir_IndiceDistanciaDocumentos(buffer_t* cod, unsigned int num, size_t b);

void comprimir_IndiceDistanciaPosiciones(buffer_t* cod, unsigned int num);

void comprimir_LexicoRepetidos(buffer_t* cod, unsigned int num);

void comprimir_LexicoDiferentes(buffer_t* cod, unsigned int num);

void comprimir_LexicoOffset(buffer_t* cod, unsigned int num);

void comprimir_BGolomb(buffer_t* cod, unsigned int num);

/* DESCOMPRESION */
unsigned int descomprimir_FrecuenciaDocumentos(debuffer_t* cod);

unsigned int descomprimir_FrecuenciaPosiciones(debuffer_t* cod);

unsigned int descomprimir_IndiceDistanciaDocumentos(debuffer_t* cod, size_t b);

unsigned int descomprimir_IndiceDistanciaPosiciones(debuffer_t* cod);

unsigned int descomprimir_LexicoRepetidos(debuffer_t* cod);

unsigned int descomprimir_LexicoDiferentes(debuffer_t* cod);

unsigned int descomprimir_LexicoOffset(debuffer_t* cod);

unsigned int descomprimir_BGolomb(debuffer_t* cod);

#endif /* TRASBORDOCODIGO_H_ */

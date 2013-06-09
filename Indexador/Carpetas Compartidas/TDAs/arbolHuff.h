#ifndef _ARBOL_HUFF_H
#define _ARBOL_HUFF_H

#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"
#include "../Manejo de Archivos/buffer.h"

#define DATO_NULO 100
#define DIR_IZQ 0
#define DIR_DER 1

typedef struct arbol_huff arbol_huff_t;

arbol_huff_t* arbol_huff_crear(size_t b);

lista_t* arbol_huff_obtener_lista_bits(arbol_huff_t* arbol,unsigned int num);

void arbol_huff_destruir(arbol_huff_t *arbol);



#endif // _ARBOL_HUFF_H

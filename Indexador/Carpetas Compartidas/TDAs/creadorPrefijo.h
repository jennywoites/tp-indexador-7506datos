#ifndef CREADORPREFIJO_H_
#define CREADORPREFIJO_H_

#include "../Manejo de Archivos/debuffer.h"
#include "lista.h"

typedef struct creadorPrefijo creadorPrefijo_t;

creadorPrefijo_t* creadorPrefijo_crear(size_t b);

lista_t* creadorPrefijo_obtener_lista_bits(creadorPrefijo_t *arbol,unsigned int num);

void creadorPrefijo_destruir(creadorPrefijo_t *arbol);

unsigned int creadorPrefijo_obtener_numero(creadorPrefijo_t* arbol,debuffer_t* debuffer);



#endif /* CREADORPREFIJO_H_ */

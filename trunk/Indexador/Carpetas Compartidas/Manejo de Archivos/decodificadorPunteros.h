#ifndef DECODIFICADOR_PUNTEROS_H_
#define DECODIFICADOR_PUNTEROS_H_

#include <stdio.h>
#include "termino.h"
#include "../TDAs/lista.h"

/* Si no logro abrir el archivo o la frecuencia del termino es 0, devuelve NULL
 * MEMORIA: Se pide memoria para cada nodo en la lista, cuando se destruya la lista
 * pasarle función "free" */
lista_t* decodificar_punteros(const char* FILE, termino_t* termino);


#endif /* DECODIFICADOR_PUNTEROS_H_ */

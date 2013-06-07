#ifndef PARSERQUERY_H_
#define PARSERQUERY_H_
#include "../../Carpetas Compartidas/TDAs/lista.h"

/* Devuelve una lista con los terminos ya parseados
 * Memoria: ademas de crear la lista, hay que recordar que se reserva memoria
 * para cada uno de los terminos (char*), por lo que, al destruir esta lista,
 * hay que pasarle FREE al destructor de la lista. */
lista_t* parserQuery_parsearConsulta(const char* query);


#endif /* PARSERQUERY_H_ */

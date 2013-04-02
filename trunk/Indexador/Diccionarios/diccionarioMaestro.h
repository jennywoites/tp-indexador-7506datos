#ifndef DICCIONARIOMAESTRO_H_
#define DICCIONARIOMAESTRO_H_
#include <stdbool.h>


void diccionario_cargar(/*const char* ruta*/); //Ver si tenemos estas palabras afuera

void diccionario_liberar();

/*Funcion que devuelve true si la palabra se la considera clave, falso si no.
 * Precondicion: el puntero no es NULL (sino, supongo que repondere false).*/
bool diccionario_palabraEsClave(char* palabra);


#endif /* DICCIONARIOMAESTRO_H_ */

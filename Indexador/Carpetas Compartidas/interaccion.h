#ifndef INTERACCION_H
#define INTERACCION_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Funcion auxiliar para ingreso de texto por teclado
// Pre: -
// Post: Devuelve un puntero a una porcion de memoria con el texto indicado.
// En caso de fallar, devuelve NULL
// Recordar: La memoria es reservada dentro de la funcion, el usuario deberia
// de liberarla.
char* leer_texto();

// Funcion auxiliar para ingreso de numeros por teclado
// Pre: -
// Post: Devuelve el numero (int).
// En caso de fallar, devuelve -1
int leer_numero();

#endif //INTERACCION_H

#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La Lista está planteada como una cola de punteros genéricos. */

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacia.
lista_t *lista_crear();

// Devuelve verdadero o falso, segun si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento a la lista, dato se encuentra al 
// principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento a la lista, dato se encuentra 
// al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista. Si la lista tiene elementos, 
// se quita el primero, y se devuelve su valor, si esta vacia, 
// devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacia.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el primer elemento de la lista. Si la lista tiene elementos, 
// se devuelve el valor del primero, si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el valor del primer elemento, cuando no esta vacia.
void *lista_ver_primero(const lista_t *lista);

// Devuelve la cantidad de elementos que tiene la lista.
// Pre: la lista fue creada.
// Post: devuelve la cantidad. Si la lista esta vacia devuelve cero.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la funcion destruir_dato 
// por parametro,para cada uno de los elementos de la lista 
// llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una funcion capaz de
// destruir los datos de la lista; NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

/*Primitivas de iteracion*/

// Crea un iterador.
// Pre: la lista fue creada
// Post: devuelve un nuevo iterador apuntando al primer lugar de la
// lista. Si no hay elementos apunta a NULL.
lista_iter_t *lista_iter_crear(const lista_t *lista);

// Apunta al proximo elemento de la lista.
// Pre: el iterador fue creado.
// Post: el iterador se movio al elemento proximo de la lista. Si esta
// al final no se mueve, devuelve falso.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el elemento de la lista al que apunta el iterador. 
// Si el iterador apunta a un nodo de la lista se devuelve el valor de
// dicho nodo, sino, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el valor del elemento al que apunta el iterador.
// Si no hay iterador devuelvo NULL.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve verdadero o falso, segun si el iterador esta o no en el 
// elemento final de la lista.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado
// Post: se elimina el iterador, no el nodo al que apunta.
void lista_iter_destruir(lista_iter_t *iter);

/* *****************************************************************
 *              PRIMITIVAS DE ITERADOR + LISTA
 * *****************************************************************/

// Agrega un nuevo elemento en el lugar en que esta el iterador. El 
// elemento del lugar en que esta el iterador se convierte en el que
// le sigue al elemento agregado. Devuelve falso en caso de error.
// Pre: la lista fue creada. el iterador fue creado.
// Post: se agrego un nuevo elemento a la lista, dato se encuentra en 
// el lugar de la lista en que esta el iterador.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);

// Quita el elemento en el lugar en que esta el iterador. Si el  
// iterador apunta a un elemento de la lista, quita el elemento y 
// devuelve su valor, sino devuelve NULL.
// Pre: la lista fue creada. el iterador fue creado.
// Post: se devolviao el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacia.
// Si borro el ultimo elemento el iterador apunta a NULL.
void *lista_borrar(lista_t *lista, lista_iter_t *iter);

#endif // LISTA_H

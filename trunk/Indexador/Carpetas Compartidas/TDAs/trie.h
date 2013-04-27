#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

//Declaracion del nombre de la estructura de trie
typedef struct trie trie_t;

//Declaracion del tipo de funcion destruir dato
typedef void (*trie_destruir_dato_t) (void *);


/* *****************************************************************
 *                    PRIMITIVAS DEL TREI
 * *****************************************************************/

/* Crea un Trie, guardando la funcion de destruccion.
 * Post: devuelve el Trie con raiz sin hijos y la funcion asignada.*/
trie_t* trie_crear(trie_destruir_dato_t dest);

/* Libera la memoria guardada por el trie.
 * Pre: el trie fue creado
 * Post: memoria liberada.*/
void trie_destruir(trie_t* trie);

/* Guarda la clave dentro del trie, con su dato asociado. En caso de ya existir la clave
 * se reemplaza el dato, utilizando la funcion de destruccion sobre el dato anterior.
 * Pre: el trie fue creado y la clave no es NULL.
 * Post: true si se cumplen las precondiciones y se pudo almacenar la clave, false en otro caso. */
bool trie_guardar(trie_t* trie, const char* clave, void* dato);

/* Elimina la clave del trie, y devuelve el dato asociado.
 * Pre: el trie fue creado, y la clave se encuentra dentro del trie.
 * Post: eliminada la clave y devuelto el dato. Si no se cumplen las precondiciones se devuelve NULL */

/* NO SE IMPLEMENTA LA PRIMITIVA DE BORRADO*/

//void* trie_borrar(trie_t* trie, const char* clave);



/* Indica si la clave pertenece al Trie.
 * Pre: el trie fue creado.
 * Post: true si la clave se encuentra en el trie, false en caso contrario, o si no se cumplen las Pre.*/
bool trie_pertenece(const trie_t* trie, const char* clave);

/* Se devuelve el dato asociado a la clave indicada.
 * Pre: el trie existe, la clave no es NULL.
 * Post: si no se cumplen las Pre, se devuelve NULL. Si la clave existe en el Trie se
 * devuelve el dato almacenado, en caso contrario se devuelve NULL*/
void* trie_obtener(const trie_t* trie, const char* clave);

/* Indica la cantidad de claves dentro del trie.
 * Pre: el trie fue creado.
 * Post: la cantidad de elementos en el trie. En caso de no existir el Trie se devuelve 0.*/
size_t trie_cantidad(const trie_t*);


#endif /* TRIE_H */

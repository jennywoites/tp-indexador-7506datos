#ifndef _HASH_H
#define _HASH_H

//ARCHIVO HEADER DEL TIPO DE DATO HASH
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;
typedef void (*hash_destruir_dato_t)(void *);


/* ******************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

//Crea un nuevo hash, recibiendo la funcion que le permita destruir los
//datos que guarda (puede recibir NULL)
//Post: hash creado
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);


//Guarda o actualiza un valor en el hash a partir de la clave y el dato
//a guardar
//Pre: el hash fue creado
//Post: el hash con una nueva clave, o con una clave actualizada
bool hash_guardar(hash_t *hash, const char *clave, void *dato);


//Elimina del hash una clave, y le devuelve el dato al usuario
//Pre: el hash fue creado. La clave existe en el hash
//Post: el hash con una clave menos, o el mismo hash si la clave no 
//estaba en el hash
void* hash_borrar(hash_t *hash, const char *clave);


//Obtiene el dato de una clave especifica
//Pre: el hash existe. La clave esta dentro del hash
//Post: NULL si las precondiciones no se cumplen, el dato pedido en  
//caso contrario
void* hash_obtener(const hash_t *hash, const char *clave);


//Informa si una clave esta dentro del Hash
//Pre: el hash fue creado
//Post: True si la clave se encuentra en el Hash, False en 
//caso contrario
bool hash_pertenece(const hash_t *hash, const char *clave);


//Devuelve la cantidad de elementos en el hash
//Pre: el hash fue creado
//Post: la cantidad de elementos. Si no hay hash, decuelve 0.
size_t hash_cantidad(const hash_t *hash);


//Se destruyen los datos del hash, y el mismo
//Pre: el hash fue creado
//Post: El hash destruido, junto con los datos que guardaba
void hash_destruir(hash_t *hash);


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR DE HASH
 * *****************************************************************/

//Crea un nuevo iterador de hash
//Pre: el hash fue creado
//Post: el iterador nuevo creado
//Si el hash esta vacio lo posiciono en la ultima lista
hash_iter_t *hash_iter_crear(const hash_t *hash);


//El iterador avanza e informa si lo ha logrado con exito
//Pre: el iterador fue creado. 
//Post: Si no se ha llegado al final del Hash, True.
//Si tras avanzar se llego al final, es decir, no esta en una
//posicion valida de lectura, devuelve False
bool hash_iter_avanzar(hash_iter_t *iter);


//Se devuelve el valor de la clave en la que esta situado el iterador
//Pre: el iterador fue creado
//Post: se devolvió la clave pedida, o NULL en caso de que el iterador 
//este en el final del hash
const char *hash_iter_ver_actual(const hash_iter_t *iter);


//Informa si el iterador de hash esta o no al final del hash
//Pre: el iterador fue creado
//Post: True si se encuentra al final, False en caso contrario
bool hash_iter_al_final(const hash_iter_t *iter);


//Se destruye el iterador
//Pre: el iterador fue creado
//Post: el iterador destruido
void hash_iter_destruir(hash_iter_t* iter);

#endif // _HASH_H

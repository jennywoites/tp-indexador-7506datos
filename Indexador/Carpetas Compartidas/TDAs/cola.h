#ifndef COLA_H
#define COLA_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La cola est� planteada como una cola de punteros gen�ricos. */

typedef struct cola cola_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// Crea una cola.
// Post: devuelve una nueva cola vac�a.
cola_t* cola_crear();

// Destruye la cola. Si se recibe la funci�n destruir_dato por par�metro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una funci�n capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void destruir_dato(void*));

// Devuelve verdadero o falso, seg�n si la cola tiene o no elementos encolados.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola);

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agreg� un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void* valor);

// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si est� vac�a devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvi� el primer elemento de la cola, cuando no est� vac�a.
void* cola_ver_primero(const cola_t *cola);

// Se obtiene la cantidad de elementos que tiene la cola.
// Pre: la cola fue creada.
// Post: se devolvio la cantidad de elementos que tiene la cola.
size_t cola_largo(cola_t* cola);

// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si est� vac�a, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvi� el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vac�a.
void* cola_desencolar(cola_t *cola);

#endif // COLA_H

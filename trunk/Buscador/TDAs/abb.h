#ifndef _ABB_H
#define _ABB_H

#include <stdlib.h>
#include <stdbool.h>

//Declaración del nombre de la estructura de arbol abb
typedef struct abb abb_t;

//Delclaración del tipo de funcion comparar
typedef int (*abb_comparar_clave_t) (const char *, const char *);

//Declaracion del tipo de funcion destruir dato
typedef void (*abb_destruir_dato_t) (void *);

//Crea el arbol, asignando las funciones de comparacion y 
//de destruccion de dato
//Pre: existe funcion de comparacion
//Post: devuelve un puntero a un arbol, con raiz en NULL y las 
//funciones asignadas
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Inserta el dato en el arbol, respetando la condicion de arbol abb
//Pre: hay arbol. La clave es distinta de NULL (no tiene sentido 
//ordenarla)
//Post: se inserto el el dato en el arbol con su clave y devuelve true.
// Si no se oudo guardar devuelve false.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//Borra del arbol el nodo al que le corresponde la clave, y devuelve 
//su dato
//Pre: hay arbol. La clave es distinta de NULL. La clave pertenece a 
//un nodo del arbol
//Post: se borra el nodo del arbol. Se conserva la propiedad de arbol.
// Devuelve el dato. Si no se encuentra el nodo devuelve NULL.
void *abb_borrar(abb_t *arbol, const char *clave);

//Devuelve el dato que le corresponde la clave
//Pre: hay arbol. La clave es distinta de NULL. La clave pertenece a 
//un nodo del arbol
//Post: Devuelve el dato que le corresponde la clave. Si no se encuentra
// la clave devuelve NULL.
void *abb_obtener(const abb_t *arbol, const char *clave);

//Indica si la clave aparece en algun nodo del arbol
//Pre: hay arbol.
//Post: devuelve true si pertenece y false en caso contrario
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Devuelve la cantidad nodos almacenados en el arbol
//Pre: hay arbol.
//Post: devuelve un numero natural indicando la cantidad. Si no hay 
//arbol o no hay nodos, devuelve 0.
size_t abb_cantidad(abb_t *arbol);

//Se encarga de eliminar toda las estructuras almacenadas en el arbol.
//Pre: hay arbol
//Post: se destruye el arbol, todos sus nodos, incluyendo claves y datos
// que aun estan almacenados.
void abb_destruir(abb_t *arbol);

//Itera inorder, aplicando a cada nodo la funcion pasada por parametro
//Pre: hay arbol. Se pasa una funcion.
//Post: a cada par clave-dato se le aplica en orden la funcion
void abb_in_order(abb_t *arbol, bool funcion(const char *, void *, void *), void *extra);

//Definicion del tipo iterador
typedef struct abb_iter abb_iter_t;

//Crea el iterador de abb
//Pre: hay arbol
//Crea y se devuelve el iterador. Si no hay arbol o no tiene nodos se 
//devuelve NULL.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

//Avanza a la siguiente posicion, es decir, nodo del arbol
//Pre: hay iterador.
//Si se puede avanzar devuelve un puntero al siguiente nodo, sino, 
//es NULL.
bool abb_iter_in_avanzar(abb_iter_t *iter);

//Devuelve la clave correspondiente al nodo en que se posiciona el 
//iterador.
//Pre: hay iterador.
//Post: Si es posicion valida devuelve la clave, sino es NULL
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

//Pregunta si el iterador esta al final, la cual ya no es posicion 
//valida de lectura
//Pre: hay iterador
//Post: true si esta al final y false en caso contrario. Si no hay 
//iterador devuelve true, ya que no es posicion valida de lectura
bool abb_iter_in_al_final(const abb_iter_t *iter);

//Se encarga de eliminar la estructura del iterador
//Pre: hay iterador
//Post: Se destruye al iteradoor. 
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // _ABB_H

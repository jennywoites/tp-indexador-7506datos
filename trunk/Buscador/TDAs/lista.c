#include "lista.h"
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/ 

/* Definimos el tipo nodo */
typedef struct nodo{
	void* dato;
	struct nodo* proximo;
}nodo_t;

/* Definimos las funciones que tienen los nodos */

nodo_t* crear_nodo(void* valor){ //crea nodo y da valores por defecto
	nodo_t* nodo=malloc(sizeof(nodo_t));
	if (!nodo)
		return NULL;
	nodo->proximo=NULL;
	nodo->dato=valor;
	return nodo;
}

/* Definimos el tipo de dato LISTA */
struct lista{
	nodo_t* primero;
	nodo_t* ultimo;		//uso para insertar ultimo en orden 1
	size_t cantidad;	//uso para hacer la funcion largo orden 1
};

/* Definimos el tipo de dato ITERADOR */
struct lista_iter{
	nodo_t* actual;		//la direccion donde esta el iterador
	nodo_t* anterior;	//la direccion del nodo anterior
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(){	//crea lista con valores por defecto
	lista_t *nueva_lista=malloc(sizeof(lista_t));
	if (!nueva_lista)
		return NULL;
	nueva_lista->ultimo=NULL;	//aun no hay nodos
	nueva_lista->primero=NULL;
	nueva_lista->cantidad=0;
	return nueva_lista;
}

bool lista_esta_vacia(const lista_t* lista){
	return (lista->cantidad==0);	//vacia corresponde cantidad 0
}

bool lista_insertar_primero(lista_t *lista, void* dato){
	if (!lista)
		return false;
	nodo_t *nuevo_nodo=crear_nodo(dato);
	if (!nuevo_nodo)
		return false;

	//con lista y nodo validos:
	if (!lista_esta_vacia(lista)){
		nuevo_nodo->proximo=lista->primero;	//el que era primero se
									//vuelve el proximo del nuevo nodo
		lista->primero=nuevo_nodo;	//inserto el nodo al principio
	}else{
		lista->primero=nuevo_nodo;	//el nodo insertado es el unico
		lista->ultimo=nuevo_nodo;
	}
	lista->cantidad++;	//aumento la cantidad, agrego un nodo
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void* dato){
	if (!lista)
		return false;
	nodo_t *nuevo_nodo=crear_nodo(dato);
	if (!nuevo_nodo)
		return false;
	//con lista y nodo validos:

	if (!lista_esta_vacia(lista)){
		lista->ultimo->proximo=nuevo_nodo;	//el que era ultimo tiene
										//como proximo el nuevo nodo
		lista->ultimo=nuevo_nodo;	//mi nuevo nodo es el ultimo
	}else{
		lista->primero=nuevo_nodo;	//el nodo insertado es el unico
		lista->ultimo=nuevo_nodo;
	}
	lista->cantidad++;	//aumento la cantidad, agrego un nodo
	return true;
}

void* lista_ver_primero(const lista_t* lista){
	if (!lista)
		return NULL;
	if (lista_esta_vacia(lista))
		return NULL;
	//si existe lista y primer elemento:
	return lista->primero->dato;	//devuelvo el dato
}

size_t lista_largo(const lista_t *lista){
	if (!lista)
		return 0;	//si no hay lista, como precondicion devuelve 0
	//si existe lista
	return lista->cantidad;	//devuelvo el largo
}

void* lista_borrar_primero(lista_t *lista){
	if (!lista)
		return NULL;
	if (lista_esta_vacia(lista))
		return NULL;
	//con lista y nodo validos:
	nodo_t *aux=lista->primero;
	if (lista->primero==lista->ultimo) //es el unico, lista queda vacia
		lista->primero=lista->ultimo=NULL;
	else
		lista->primero=aux->proximo;	//quito el primer nodo, queda 
									//el segundo nodo como nuevo primero
	
	void* valor=aux->dato;
	free(aux);	//se libera la memoria del nodo
	lista->cantidad--;	//disminuyo la cantidad, quito un nodo
	return valor;
}


void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	void* aux;
	while (!lista_esta_vacia(lista)){
		aux=lista_borrar_primero(lista);
		//borro el primero sucesivamente
		if (destruir_dato)	//ademas destruyo el dato
			destruir_dato(aux);
	}
	free(lista);	//libero la memoria de la lista
}


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/
 
lista_iter_t* lista_iter_crear(const lista_t *lista){
	lista_iter_t* iterador=malloc(sizeof(lista_iter_t));
	if (!iterador)
		return NULL;
	//crea iterador con valores por defecto
	iterador->actual=lista->primero;
	iterador->anterior=NULL;
	return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if (!iter)
		return false;
	if (lista_iter_al_final(iter))
		return false;
	//para iterador que no esta al final:
	iter->anterior=iter->actual;	//el nuevo anterior es el actual
	iter->actual=iter->actual->proximo;	//el nuevo actual es el prox.
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (!iter)
		return NULL;
	if (lista_iter_al_final(iter))
		return NULL;
	//para iterador que no esta al final:
	return iter->actual->dato;	//devuelvo el dato
}

bool lista_iter_al_final(const lista_iter_t *iter){
	if(!iter)	// Si no hay iterador devuelvo true;
		return true;
	return (iter->actual==NULL);	//esta al final si el actual apunta
									// a NULL. El anterior apunta al 
									//ultimo nodo valido
}

void lista_iter_destruir(lista_iter_t *iter){
	if(iter!=NULL)
		free(iter);	//a iterador creado, libero su memoria
}


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA CON EL ITERADOR
 * *****************************************************************/

bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato){
	if(!iter)
		return false;
	//a iterador creado:
		
	if (iter->anterior==NULL){
		//si estoy en el primer lugar: llamo a insertar primero.
		if(!lista_insertar_primero(lista,dato))
			return false;
		iter->actual=lista->primero;
		//el actual es el que acabo de agregar, el primero
		return true;
	}

	if(lista_iter_al_final(iter)){
	//si estoy en el ultimo lugar: llamo a insertar ultimo
		if(!lista_insertar_ultimo(lista,dato))
			return false;
		iter->actual=lista->ultimo;
		//el actual es el que acabo de agregar, el ultimo
		return true;
	}
	//si esta en algun lugar del medio:
	nodo_t* nodo=crear_nodo(dato);	//creo un nuevo nodo
	if (!nodo)
		return false;

	//a nodo creado valido:
	iter->anterior->proximo=nodo;	//el nodo nuevo 
								//es el proximo del nodo anterior

	nodo->proximo=iter->actual;		//el proximo del nodo es el que 
								//estaba en el lugar en el que inserte

	nodo->dato=dato;		//asigno el dato
	iter->actual=nodo;	//me paro en el nodo nuevo
	lista->cantidad++;	//aumento la cantidad, agrego un nodo
	return true;
}

void *lista_borrar(lista_t *lista, lista_iter_t *iter){
	if(!iter)
		return NULL;
	if (lista_iter_al_final(iter))
		return NULL;
	
	//a iterador creado, posicion valida:
	void* valor=iter->actual->dato;	//tomo el valor a devolver
	
	if(iter->anterior==NULL){
		//me posiciono en el nodo proximo al borrado
		iter->actual=iter->actual->proximo;
		//si estoy en el primer lugar: llamo a borrar primero:
		if(!lista_borrar_primero(lista))
			return NULL;
		return valor;
	}
	
	//al anterior borrado le asigno el proximo al borrado:
	iter->anterior->proximo=iter->actual->proximo;
	free(iter->actual);	//libero la memoria del nodo borrado
	
	//me posiciono en el nodo proximo al borrado:
	//(si estaba en el ultimo lugar valido, el proximo nodo es NULL
	//lo que es correcto. Si borro el ultimo nodo quedo parado en NULL)
	iter->actual=iter->anterior->proximo;
	lista->cantidad--;	//disminuyo la cantidad, quito un nodo
	return valor;
}

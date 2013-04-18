#include <stdlib.h>
#include "pila.h"

#define STDTAM 32

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/
 
struct pila {
	void** datos;
	size_t tamanio;
	size_t cantidad;
};

/* ******************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/

pila_t* pila_crear(){
	pila_t* nueva_pila=malloc(sizeof(pila_t));
	if (nueva_pila==NULL)
		return NULL;
	nueva_pila->tamanio=STDTAM;
	nueva_pila->cantidad=0;
	nueva_pila->datos=malloc(nueva_pila->tamanio*sizeof(void*));
        
	if (nueva_pila->datos==NULL){
		free(nueva_pila);
		return NULL;
	}    

	return nueva_pila;
}

bool pila_esta_vacia(const pila_t *pila){
	if (pila==NULL)
		return true;
	if (pila->cantidad==0)
		return true;
	return false;
}
 
void pila_cambiar_tamanio(pila_t* pila, size_t nuevotam){
	void* datosNuevos=realloc(pila->datos, nuevotam*sizeof(void*));
     
	if (datosNuevos==NULL){ 
		free(datosNuevos); 
	}else{
		pila->datos=datosNuevos;
		pila->tamanio=nuevotam;
	}
}

bool pila_apilar(pila_t* pila, void* valor){
	if (pila==NULL) return false;
     
	if (pila->cantidad==pila->tamanio) {
		bool sePuede; int tamOriginal=pila->tamanio;
		pila_cambiar_tamanio(pila, pila->tamanio+STDTAM);
		sePuede=tamOriginal<pila->tamanio;
		if (!sePuede) return false;
	}
	(pila->datos)[pila->cantidad]=valor;
	pila->cantidad++;
	return true;
}


void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila))
		return NULL;
	return pila->datos[pila->cantidad-1];
}

void* pila_desapilar(pila_t* pila){
	void* elementoTope=pila_ver_tope(pila);

	if (elementoTope==NULL) return NULL;

	pila->cantidad--;

	if (pila->cantidad<=pila->tamanio/2 && (pila->tamanio - pila->cantidad) > STDTAM )
		pila_cambiar_tamanio(pila,pila->tamanio-STDTAM);

	return elementoTope;
}

void pila_destruir(pila_t *pila, void destruir_dato(void*)){ 
	void* aux;
	while (!pila_esta_vacia(pila)){
		aux=pila_desapilar(pila);
		if (destruir_dato)
		destruir_dato(aux);
	}
	free(pila->datos);
	free(pila);
}

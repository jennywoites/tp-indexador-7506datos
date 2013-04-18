#include "abb.h"
#include "pila.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/ 

/* Definimos el tipo Arbol abb */
typedef struct nodo_abb{
	char* clave;
	void* dato;
	struct nodo_abb* izq;
	struct nodo_abb* der;
}nodo_abb_t;

/* Definimos el tipo nodo Arbol abb */
struct abb{
	nodo_abb_t* raiz;
	abb_comparar_clave_t fcmp;
	abb_destruir_dato_t fdestruir;
	int cant;
};

//Crea el arbol, asignando las funciones de comparacion y de destruccion
// de dato
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	//Pido memoria para la estructura de arbol
	abb_t* arbol=malloc(sizeof(abb_t));
	if (!arbol) return NULL;

	//No apunta a ninguna raiz
	arbol->raiz=NULL;
	//Si no hay funcion de comparacion, no podemos hacer nada
	if (!cmp)	return NULL;
	//Se guardan las funciones
	arbol->fcmp=cmp;
	arbol->fdestruir=destruir_dato;
	arbol->cant=0;
	return arbol;
}

//Crea el nodo con la clave y el dato correspondientes
nodo_abb_t* nodo_abb_crear(const char* clave, void* dato){
	//Pido memoria para la estructura de nodo
	nodo_abb_t* nodo=malloc(sizeof(nodo_abb_t));
	if (!nodo) return NULL;

	//Pido memoria para la copia interna de la clave
	nodo->clave = malloc(sizeof(char)*(strlen(clave)+1));
	if (!nodo->clave){
		free(nodo);
		return NULL;
	}

	//Copio la clave
	strcpy(nodo->clave,clave);
	//No tiene hijos
	nodo->der=NULL;
	nodo->izq=NULL;
	//Le copio el dato
	nodo->dato=dato;

	return nodo;
}

//Guarda un nodo, uniendolo al arbol
bool nodo_abb_guardar(abb_t* arbol, nodo_abb_t *nodo, const char *clave, void *dato,nodo_abb_t *padre,char lado){
	//que no haya nodo implica que encontre una hoja, y puedo guardar
	if (!nodo){
		//creo nodo
		nodo=nodo_abb_crear(clave,dato);
		if (!nodo)	return false;
		//se lo inserto al padre del lado correcto
		if (lado=='d')
			padre->der=nodo;
		if (lado=='i')
			padre->izq=nodo;
		arbol->cant++;
		return true;
	}

	if (arbol->fcmp(nodo->clave,clave)==0){
		//destruyo dato anterior
		if (nodo->dato && arbol->fdestruir)	arbol->fdestruir(nodo->dato);
		//actualizo el dato
		nodo->dato = dato;
		return true;
	}

	//aplico recursividad hacia el lado correcto segun la clave, 
	//acompañando con la informacion
	//del lado al que se va para poder actualizar correctamente al padre
	if (arbol->fcmp(nodo->clave,clave)>0){
		lado='i';
		return nodo_abb_guardar(arbol,nodo->izq,clave,dato,nodo,lado);
	}
	lado='d';
	return nodo_abb_guardar(arbol,nodo->der,clave,dato,nodo,lado);
}

//Inserta el dato en el arbol, respetando la condicion de arbol abb. 
//Funciona como mascara a la funcion de guardado recursiva
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	//si no hay clave, no se puede guardar
	if (!clave)	return NULL;
	nodo_abb_t* nodo=arbol->raiz;
	//si no hay nodo, implica que el arbol esta vacio
	if(!nodo){
		//creo el primer nodo, sus hijos son NULL
		nodo=nodo_abb_crear(clave,dato);
		if(!nodo)	return false;
		//la raiz apuntara al nodo
		arbol->raiz = nodo;
		arbol->cant++;
		return true;
	}
	return nodo_abb_guardar(arbol,arbol->raiz,clave,dato,NULL,'n');
}

//Borra del arbol el nodo al que le corresponde la clave, y devuelve 
//su dato
void* borrado_nodo(nodo_abb_t* nodo,const char* clave, abb_t* arbol, nodo_abb_t* padre, char dir){
	if (!nodo) return NULL;
	//llamada recursiva a la izquierda
	if(arbol->fcmp(nodo->clave,clave)>0)
		return borrado_nodo(nodo->izq, clave, arbol, nodo, 'i');
	//llamada recursiva a la derecha
	if(arbol->fcmp(nodo->clave,clave)<0)
		return borrado_nodo(nodo->der, clave, arbol, nodo, 'd');

	void*devolver=nodo->dato;

	//estoy en el que quiero borrar:
		
	bool dos_hijos=true;
	
	//casos raiz con un/ningun hijo:
	nodo_abb_t* hijo;
	if(!nodo->der && !nodo->izq){
		dos_hijos=false;
		hijo=NULL;	//no hay hijo
	}
	if(nodo->der && !nodo->izq){
		dos_hijos=false;
		hijo=nodo->der;	//el unico hijo es el derecho
	}
	if(nodo->izq && !nodo->der){
		dos_hijos=false;
		hijo=nodo->izq;	//el unico hijo es el izquierdo
	}

	if(!dos_hijos){
		if (dir=='i'){
			padre->izq=hijo;
		}

		if(dir=='d'){
			padre->der=hijo;
		}

		if(dir=='r'){
			arbol->raiz=hijo;
		}

		arbol->cant--;
		free(nodo->clave);
		free(nodo);
		return devolver;
	}

	//cualquier caso que tenga dos hijos
	//Busco reemplazo:	busca el maximo a la izquierda
	//me desplazo lo mas al maximo posible
	nodo_abb_t* nodo_reemplazo=nodo->izq;
	while(nodo_reemplazo->der)
		nodo_reemplazo=nodo_reemplazo->der;
	//este es el nodo que resulta ser el maximo
	
	char* clave_reemplazo=malloc(sizeof(char)*(strlen(nodo_reemplazo->clave)+1));
	strcpy(clave_reemplazo,nodo_reemplazo->clave);

	void* dato_reemplazo=abb_borrar(arbol,clave_reemplazo);
	//este borrar va al borrado_nodo, y cae en el caso gral con uno o 
	//sin hijos 
	//es bueno volver a buscarlo ya que uno se asegura de que es 
	//quitado de su posicion correctamente
	
	//libero el nodo
	free(nodo->clave);
	//se hace el reemplazo de clave y dato, los demas atributos 
	//se conservan
	nodo->clave=clave_reemplazo;
	nodo->dato=dato_reemplazo;
	//en este caso no hace falta disminuir el valor de cantidad, ya 
	//que ya se hizo cuando se llamo a borrar el reemplazo
	return devolver;
	}

//Borra del arbol el nodo al que le corresponde la clave, y devuelve
// su dato. Funciona como mascara del borrado recursivo
void* abb_borrar(abb_t* arbol, const char* clave){
	if(!arbol) return NULL;
	if(!clave) return NULL;

	return borrado_nodo(arbol->raiz, clave, arbol, NULL,'r');
}

//Devuelve el nodo que le corresponde la clave, si se encuentra
//Funcion usada como generalizacion de abb_obtener y abb_pertenece
nodo_abb_t* nodo_abb_obtengo(const abb_t *arbol, nodo_abb_t* nodo, const char *clave){
	if (!nodo)	return NULL;
	//soy yo = devulevo dato
	if (arbol->fcmp(nodo->clave,clave)==0)
		return nodo;
	//lamada recursiva
	if (arbol->fcmp(nodo->clave,clave)>0)
		return nodo_abb_obtengo(arbol,nodo->izq,clave);
	return nodo_abb_obtengo(arbol,nodo->der,clave);
}

//Devuelve el dato que le corresponde la clave.
void* nodo_abb_obtener(const abb_t *arbol, nodo_abb_t* nodo, const char *clave){
	if (!nodo)	return NULL;
	//llama a la funcion que obtiene el nodo pedido, o NULL si no esta
	nodo_abb_t* obtenido = nodo_abb_obtengo(arbol,nodo,clave);
	if (!obtenido)	//si no esta, devuelvo NULL
		return NULL;
	return obtenido->dato;	//si esta, devuelvo el Dato
}

//Indica si la clave aparece en algun nodo del arbol.
bool nodo_abb_pertenece(const abb_t *arbol, nodo_abb_t* nodo, const char *clave){
	if (!nodo)	return false;
	//llama a la funcion que obtiene el nodo pedido, o NULL si no esta
	nodo_abb_t* obtenido = nodo_abb_obtengo(arbol,nodo,clave);
	
	if (!obtenido)	//si no esta, no pertenece
		return false;
	return true;	//si esta, pertenece
}

//Devuelve el dato que le corresponde la clave. Funciona como mascara 
//a la funcion de obtener recursiva
void* abb_obtener(const abb_t *arbol, const char *clave){
	if (!arbol)	return NULL;
	nodo_abb_t* nodo=arbol->raiz;
	if (!nodo)	return NULL;
	if (!clave)	return NULL;
	//lamada recursiva
	return nodo_abb_obtener(arbol,nodo,clave);
}

//Indica si la clave aparece en algun nodo del arbol. Funciona como 
//mascara a la funcion de pertenece recursiva
bool abb_pertenece(const abb_t *arbol, const char *clave){
	if (!arbol)	return false;
	nodo_abb_t* nodo=arbol->raiz;
	if (!nodo)	return false;
	if (!clave)	return false;
	//lamada recursiva
	return nodo_abb_pertenece(arbol,nodo,clave);
}

//Devuelve la cantidad nodos almacenados en el arbol. Funciono como 
//mascara a la funcion de cantidad recursiva
size_t abb_cantidad(abb_t *arbol){
	if(!arbol)	return 0;

	return arbol->cant;
}

//Se encarga de eliminar toda las estructuras almacenadas en el nodo.
void nodo_abb_destruir(abb_t *arbol, nodo_abb_t *nodo){
	//si no hay nodo, no hay qué destruir
	if(!nodo)	return;
	//detruyo el hijo izq
	nodo_abb_destruir(arbol,nodo->izq);
	//detruyo el hijo der
	nodo_abb_destruir(arbol,nodo->der);
	//libero la clave
	free(nodo->clave);
	//destruyo el dato
	if (nodo->dato && arbol->fdestruir)	arbol->fdestruir(nodo->dato);
	//libero la estructura nodo
	free(nodo);
}

//Se encarga de eliminar toda las estructuras almacenadas en el arbol.
void abb_destruir(abb_t *arbol){
	//si no hay nodo, no hay qué destruir
	if(!arbol)	return;
	//tomo la raiz
	nodo_abb_t* nodo=arbol->raiz;
	if(!nodo){
		free(arbol);
		return;
	}
	//detruyo el hijo izq
	nodo_abb_destruir(arbol,nodo->izq);
	//detruyo el hijo der
	nodo_abb_destruir(arbol,nodo->der);
	//libero la clave
	free(nodo->clave);
	//destruyo el dato
	if (nodo->dato && arbol->fdestruir)	arbol->fdestruir(nodo->dato);
	//libero la estructura nodo
	free(nodo);
	//libero la estructura arbol
	free(arbol);
}

/* Definimos el tipo Iterador de Arbol (en IN ORDER)*/

struct abb_iter{
	pila_t* pila_iter;
};

//Crea el iterador de abb
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	if(!arbol) return NULL;
	nodo_abb_t* nodo = arbol->raiz;

	abb_iter_t* iter=malloc(sizeof(abb_iter_t));
	if(!iter) return NULL;

	iter->pila_iter=pila_crear();
	if(!iter->pila_iter){
		free(iter);
		return NULL;
	}

	if(!nodo)	return iter;
	
	//Apilo la raiz, y todos sus hijos izquierdos
	while (nodo!=NULL){
		pila_apilar(iter->pila_iter, (void*) nodo);
		nodo=nodo->izq;
	}
	
	return iter;
}

//Pregunta si el iterador esta al final, la cual ya no es posicion 
//valida de lectura
bool abb_iter_in_al_final(const abb_iter_t *iter){
	if (!iter)	return false;
	return pila_esta_vacia(iter->pila_iter);
}

//Avanza a la siguiente posicion, es decir, nodo del arbol
bool abb_iter_in_avanzar(abb_iter_t *iter){
	if(!iter) return false;

	if(abb_iter_in_al_final(iter)) return false;

	const nodo_abb_t* actual=pila_desapilar(iter->pila_iter);

	// me fijo si tiene hijos derechos, si no tiene ya termino de 
	//avanzar con haber desapilado. si tiene un hijo derecho, lo 
	//apilo y todos sus hijos izquierdos
	actual=actual->der;

	while (actual!=NULL){
		pila_apilar(iter->pila_iter,(void*)actual);
		actual=actual->izq;
	}
	return true;
}

//Devuelve la clave correspondiente al nodo en que se posiciona el 
//iterador.
const char* abb_iter_in_ver_actual(const abb_iter_t *iter){
	if(!iter) return NULL;
	if (abb_iter_in_al_final(iter)) return NULL;

	const nodo_abb_t* actual=pila_ver_tope(iter->pila_iter);
	return actual->clave;
}

//Se encarga de eliminar la estructura del iterador
void abb_iter_in_destruir(abb_iter_t* iter){
	if(!iter) return;

	pila_destruir(iter->pila_iter,NULL);
	
	free(iter);
}

void abb_nodo_in_order(nodo_abb_t *nodo, bool funcion(const char *, void *, void *), void *extra){
	if(!nodo) return;

	abb_nodo_in_order(nodo->izq,funcion,extra);
	
	funcion(nodo->clave, nodo->dato, extra);	//aplica inorder

	abb_nodo_in_order(nodo->der,funcion,extra);

}

void abb_in_order(abb_t *arbol, bool funcion(const char *, void *, void *), void *extra){
	if (!arbol)	return;
	
	abb_nodo_in_order(arbol->raiz,funcion,extra);

}

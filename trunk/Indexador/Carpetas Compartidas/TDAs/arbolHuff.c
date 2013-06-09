#include <stdlib.h>
#include "arbolHuff.h"
#include <stdbool.h>
#include <string.h>

typedef struct nodo_huff{
	char* clave;
	unsigned int dato;
	struct nodo_huff* izq;
	struct nodo_huff* der;
}nodo_arbol_huff_t;

struct arbol_huff{
	nodo_arbol_huff_t* raiz;
	cmp_func_t fcmp;
};

void crear_con_base_de_arbol(arbol_huff_t* arbol, size_t b);
nodo_arbol_huff_t* arbol_huff_unir_nodos(nodo_arbol_huff_t* nodoIzq, nodo_arbol_huff_t* nodoDer);
nodo_arbol_huff_t* nodo_arbol_huff_crear(const char* clave, unsigned int dato);


//Crea el arbol, asignando las funciones de comparacion y de destruccion
// de dato
arbol_huff_t* arbol_huff_crear(cmp_func_t cmp, size_t b){
	//Si no hay funcion de comparacion, no podemos hacer nada
	if (!cmp)	return NULL;

	arbol_huff_t* arbol = malloc(sizeof(arbol_huff_t));
	if (!arbol) return NULL;

	arbol->fcmp=cmp;
	arbol->raiz=NULL;

	crear_con_base_de_arbol(arbol,b);

	return arbol;

}

void crear_con_base_de_arbol(arbol_huff_t* arbol, size_t b){
	if(b== 0 || !arbol)
		return;

	heap_t* heap = heap_crear(arbol->fcmp);
	if(!heap)
		return;

	char* clave;

	//FIXME: 6??
	clave = malloc(sizeof(char) * 6);

	if(!clave)
		return;

	strcpy(clave, "1");

	unsigned int dato;
	for(unsigned int i=0; i<b;i++){
		dato = i;
		heap_encolar(heap, (void*) nodo_arbol_huff_crear(clave,dato) );
	}

	nodo_arbol_huff_t* nodoIzq;
	nodo_arbol_huff_t* nodoDer;

	while(heap_cantidad(heap) > 1){
		nodoIzq = heap_desencolar(heap);
		nodoDer = heap_desencolar(heap);
		//printf("izq: %s \n",nodoIzq->clave);
		//printf("der: %s \n",nodoDer->clave);
		heap_encolar(heap, arbol_huff_unir_nodos(nodoIzq,nodoDer));
	}

	arbol->raiz = heap_desencolar(heap);
	printf("raiz: %s \n",arbol->raiz->clave);

	free(clave);
	heap_destruir(heap, NULL);

}


//Crea el nodo con la clave y el dato correspondientes
nodo_arbol_huff_t* nodo_arbol_huff_crear(const char* clave, unsigned int dato){
	//Pido memoria para la estructura de nodo
	nodo_arbol_huff_t* nodo = malloc(sizeof(nodo_arbol_huff_t));
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


nodo_arbol_huff_t* arbol_huff_unir_nodos(nodo_arbol_huff_t* nodoIzq, nodo_arbol_huff_t* nodoDer){
	if(!nodoIzq || !nodoDer)
		return NULL;

	//FIXME: ojo con esto
	unsigned int numIzq = atoi(nodoIzq->clave);
	unsigned int numDer = atoi(nodoDer->clave);
	unsigned int suma = numIzq + numDer;

	char* claveUnion = malloc(sizeof(char)* 6);

	//itoa(suma, claveUnion, 10);

	sprintf(claveUnion,"%u", suma);

	nodo_arbol_huff_t* nodoUnion = nodo_arbol_huff_crear(claveUnion,DATO_NULO);

	nodoUnion->izq = nodoIzq;
	nodoUnion->der = nodoDer;

	free(claveUnion);

	return nodoUnion;
}

bool encontrar_en_arbol(nodo_arbol_huff_t* nodo, unsigned int num, lista_t* lista){

	if(!nodo)
		return false;

	//printf("encontrar: %u \n",nodo->dato);
	if(nodo->dato == num)
		return true;

	Byte_t* direccion = malloc(sizeof(Byte_t));
	if(!direccion)
		return false;

	if(encontrar_en_arbol(nodo->izq,num,lista)){
		*direccion = DIR_IZQ;
		lista_insertar_primero(lista,(void*)direccion);
		return true;
	}


	if(encontrar_en_arbol(nodo->der,num,lista)){
		*direccion = DIR_DER;
		lista_insertar_primero(lista,(void*)direccion);
		return true;
	}

	free(direccion);
	return false;

}


lista_t* arbol_huff_obtener_lista_bits(arbol_huff_t* arbol,unsigned int num){

	lista_t* lista = lista_crear();

	if(!lista)
		return NULL;

	encontrar_en_arbol(arbol->raiz,num,lista);

	return lista;
}

//Se encarga de eliminar toda las estructuras almacenadas en el nodo.
void nodo_arbol_huff_destruir(nodo_arbol_huff_t *nodo){
	//si no hay nodo, no hay qué destruir
	if(!nodo)	return;
	//detruyo el hijo izq
	nodo_arbol_huff_destruir(nodo->izq);
	//detruyo el hijo der
	nodo_arbol_huff_destruir(nodo->der);
	//libero la clave
	free(nodo->clave);
	//libero la estructura nodo
	free(nodo);
}

//Se encarga de eliminar toda las estructuras almacenadas en el arbol.
void arbol_huff_destruir(arbol_huff_t *arbol){
	//si no hay nodo, no hay qué destruir
	if(!arbol)	return;
	//tomo la raiz
	nodo_arbol_huff_t* nodo = arbol->raiz;
	if(!nodo){
		free(arbol);
		return;
	}
	//detruyo el hijo izq
	nodo_arbol_huff_destruir(nodo->izq);
	//detruyo el hijo der
	nodo_arbol_huff_destruir(nodo->der);
	//libero la clave
	free(nodo->clave);
	//libero la estructura nodo
	free(nodo);
	//libero la estructura arbol
	free(arbol);
}

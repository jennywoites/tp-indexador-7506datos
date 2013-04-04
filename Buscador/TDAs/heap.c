#include "heap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#define STDTAM 32
#define DIFERENCIA 6

/* Definimos el tipo Heap */

struct heap_t{
	void** vector;
	size_t cantidad;
	size_t tamanio;
	cmp_func_t fcomparar;

};


//Primitivas del heap

// Crea un heap. Recibe como único parámetro la función de comparación
// a utilizar.
// Pre: hay función de comparación.
// Post: Devuelve el heap o NULL es caso de Error o si no hay función 
// comparación.
 heap_t* heap_crear(cmp_func_t cmp){
	if(!cmp) return NULL; //tengo que tener funcion de compararcion

	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;

	heap->vector=malloc(sizeof(void*)*STDTAM);
	if(!heap->vector){
		free(heap);
		return NULL;
	}
	int i;
	for (i = 0; i < STDTAM; i++ )
		heap->vector[i] = NULL;

	heap->tamanio = STDTAM;
		heap->cantidad = 0;
	heap->fcomparar = cmp;
	
	return heap;
}
// Elimina el heap, llamando a la función dada para cada elemento del 
// mismo.
// Pre: hay funcion de destruccion o NULL si no se utiliza.
// Post: se destruye el heap. Si existe, se aplica la funcion de 
// destruccion a todos los elementos.
void heap_destruir(heap_t* heap,void destruir_elemento(void *e)){
	if (!heap) return;

	int i;
	//por cada elemento aplica la funcion de destruccion, si existe
	for (i = 0; i < heap->cantidad; i++)
		if(destruir_elemento)	destruir_elemento(heap->vector[i]);

	free(heap->vector);
	free(heap);
}

// Devuelve la cantidad de elementos que hay en el heap.
// Pre: el heap fue creado.
// Post: se devuelve la cantidad o cero si no hay heap.
size_t heap_cantidad(const heap_t* heap){
	if(!heap) return 0;
	else return heap->cantidad;
}

// Devuelve true si la cantidad de elementos es cero.
// Pre: el heap fue creado., sino no se puede tratar.
// Post: se devuelve si esta vacio.
bool heap_esta_vacio(const heap_t* heap){
	// como no sabria que decir si el heap no existe, como precondicion
	// digo que fue creado
	return heap->cantidad==0;
}

// Devuelve el elemento con máxima prioridad. Si el heap esta vacío, 
// devuelve NULL. 
// Pre: el heap fue creado.
// Post: se devuelve el elemento con máxima prioridad o, si el 
// esta vacío, NULL. 
void* heap_ver_max(const heap_t* heap){
	if(heap_cantidad(heap)==0)	return NULL;
	//aqui tambien se contempla que el heap no sea NULL

	return heap->vector[0];           
}

//Funcion que redimensiona el Heap.
//Pre: el heap fue creado. Llega como parametro el nuevo tamaño
//Post: devuelve true si logra redimensionar sin problemas, o false 
//en caso contrario
bool heap_cambiar_tamanio(heap_t* heap, size_t nuevoTam){
	if (!heap) return false;
	if (nuevoTam<STDTAM) return false;
	
	//se pide la nueva memoria
	void** vector_nuevo=realloc(heap->vector, sizeof(void*)*nuevoTam);
	if(!vector_nuevo) return false;

	//se utiliza el nuevo vector creado
	heap->vector=vector_nuevo;
	heap->tamanio=nuevoTam;
	return true;
}

void heap_upheap(heap_t* heap, size_t pos);
void heap_downheap(heap_t* heap, size_t pos);

//Agrega un elemento al heap.
//Pre: El elemento no puede ser NULL.
//Post: Devuelve true si se agregó un nuevo elemento al heap, false 
//en caso contrario
bool heap_encolar(heap_t* heap, void* elem){
	if (!heap) return false;
	if(!elem) return false;

	bool sepuede=true;
	if(heap->tamanio==heap->cantidad)
		//caso de redimension
		sepuede=heap_cambiar_tamanio(heap,heap->tamanio*2);

	if(!sepuede) return false;

	//se agrega el elemento
	heap->vector[heap->cantidad]=elem;
	heap->cantidad++;
	//se actualiza la invariante de cantidad
	heap_upheap(heap,heap->cantidad-1);
	return true;
}
//Elimina el elemento con máxima prioridad, y lo devuelve.
//Pre: el heap fue creado.
//Post: el elemento desencolado ya no se encuentra en el heap. Si 
//esta vacio devuelve NULL 
void* heap_desencolar(heap_t* heap){
	if(heap_cantidad(heap)==0) return NULL;

	void* devolver=heap->vector[0];
	//se actualiza la invariante de cantidad
	heap->cantidad--;
	//sube el ultimo elemento (aqui se rompe temporalmente la condicion
	// de heap
	heap->vector[0]=heap->vector[heap->cantidad];
	heap->vector[heap->cantidad]=NULL;
	
	if(heap->tamanio >= DIFERENCIA*heap->cantidad && heap->tamanio>STDTAM)
		//caso de redimension
		heap_cambiar_tamanio(heap,heap->tamanio/2);
	
	//se hace downheap para lograr la condicion de heap
	heap_downheap(heap,0);
	return devolver;      
}

//Funcion de swap.
void swap(void** x, void** y){
	void* aux=*x;
	*x=*y;
	*y=aux;
}

//Funcion que toma un elemento agregado y lo eleva en el heap hasta
// que se encuentre en una posicion donde respete la condicion de Heap
void heap_upheap(heap_t* heap, size_t pos){
	//el unico caso en el que no tengo padre es estar en la primera 
	//posicion
	if (pos==0) return;
	
	size_t padre=(pos-1)/2;
	
	if(heap->fcomparar(heap->vector[pos],heap->vector[padre])>0){
		//si debe esta por encima de su padre, se hace el swap		
		swap(&heap->vector[pos],&heap->vector[padre]);
		//se hace upheap con el mismo elemento, que ahora esta en la 
		//posicion de padre
		heap_upheap(heap,padre);
	}
}

//Funcion para la comparacion de hermano mayor
size_t valor_max(cmp_func_t cmp, void* hermano1, size_t posHermano1, void* hermano2, size_t posHermano2){
	if(cmp(hermano1,hermano2)>=0)
		return posHermano1;
	else	return posHermano2;
}

//Funcion que a partir de un elemento se encarga de reordenar los 
//elementos tal que se cumpla la condicion de Heap
void heap_downheap(heap_t* heap, size_t pos){
	size_t hijo_der = (pos + 1) * 2;
	size_t hijo_izq = hijo_der - 1;

	// caso en el que no tiene hijos (si no tiene hijo izquierdo
	// tampoco derecho):
	if(hijo_izq >= heap->cantidad)          
		return;

	//caso en el que solo tengo hijo izquierdo
	if(hijo_der >= heap->cantidad){
		if(heap->fcomparar(heap->vector[pos], heap->vector[hijo_izq])<0){
			//si corresponde, se hace el swap
			swap(&heap->vector[pos],&heap->vector[hijo_izq]);
			//se hace downheap con el mismo elemento, que ahora 
			//esta en la posicion de hijo izq
			//si hubo un reemplazo, es posible que hayan 
			//mas (llamada recursiva)
			heap_downheap(heap, hijo_izq);
		}
		return;
	}

	// caso en el que tengo los dos hijos, veo si alguno de los hijos
	// es mayor.
	// si es verdad, simplemente reemplazo con el mayor entre ambos
	void* prioridadP=heap->vector[pos];
	void* prioridadHI=heap->vector[hijo_izq];
	void* prioridadHD=heap->vector[hijo_der];

	if(heap->fcomparar(prioridadP,prioridadHI)<0 || heap->fcomparar(prioridadP,prioridadHD)<0){
		//si corresponde, se hace el swap, con el hijo mayor
		size_t hermano_max=valor_max(heap->fcomparar,heap->vector[hijo_der],hijo_der,heap->vector[hijo_izq],hijo_izq);
		swap(&heap->vector[pos],&heap->vector[hermano_max]);
		//se hace downheap con el mismo elemento, que ahora esta en la 
		//posicion de hijo maximo
		//si hubo un reemplazo, es posible que hayan
		// mas (llamada recursiva)
		heap_downheap(heap,hermano_max);
	}

}

//Funcion que toma un vector de elementos y los acomoda en un Heap, 
//tal que cumplan la condicion de Heap
heap_t* heap_heapify(void* vec[], size_t cant, cmp_func_t cmp){
	heap_t* heap=heap_crear(cmp);
	if(!heap) return NULL;

	//toma el vector que crea el Heap y lo reemplaza con el que llega
	//por parametro
	free(heap->vector);
	heap->vector=vec;
	heap->cantidad=cant;
	heap->tamanio=cant;

	int pos=(cant/2) - 1;
	while (pos>=0){
		//realiza el downheap para la primera mitad de elementos
		heap_downheap(heap,pos);
		pos--;
	}

	return heap;
}

//Función de heapsort genérica. Esta función ordena usando el 
//heap un arreglo de punteros
//Pre: se requiere que se una función de comparación. 
//Post: devuelve el mismo vector, ordenado.
void heapsort(void *elementos[], size_t cant, cmp_func_t cmp){
	//Le creo un heap, y le digo al heap que su vector es el que quiero
	//ordenar
	heap_t* heap=heap_heapify(elementos,cant,cmp);

	//por cada pasada hasta cantidad 1:
	while (heap->cantidad>1){
		heap->cantidad--;
		//hace un swap entre el primer elemento (un maximo) y el
		//elemento que ya no sera tratado (ya quedo ordenado)
		swap(&heap->vector[0],&heap->vector[heap->cantidad]);
		//hace downheap para conservar la condicion de Heap, y tener 
		//el maximo en la primera posicion del Heap
		heap_downheap(heap,0);
	}
	
	//destruyo el heap, sin liberar el vector, pues el usuario es el
	//que lo tiene y yo solo se lo ordeno
	free(heap);
}
//Compara dos valores con la funcion de comparacion del heap
int heap_comparar(heap_t* heap, void* comparacion1,void* comparacion2){
	return heap->fcomparar(comparacion1,comparacion2);
}

#include "creadorPrefijo.h"
#include <stdlib.h>
#include <stddef.h>
#include "heap.h"
#include "lista.h"
#include <stdbool.h>

struct creadorPrefijo{
	lista_t** listas;
	size_t cantidad;
	size_t* cero;
	size_t* uno;
};

int comparacionClasesEquivalencia (const void* a, const void* b){
	lista_t* l1 = (lista_t*) a;
	lista_t* l2 = (lista_t*) b;
	return lista_largo(l2) - lista_largo(l1);
}

void vaciarYAgregar(lista_t* sacar, lista_t** indicar, size_t* valor, lista_t* laUnion){
	while (!lista_esta_vacia(sacar)){
		size_t* num = lista_borrar_primero(sacar);
		lista_insertar_primero(indicar[*num], valor);
		lista_insertar_ultimo(laUnion, num);
	}
}


creadorPrefijo_t* creadorPrefijo_crear(size_t b){
	if (b == 0) return NULL;
	creadorPrefijo_t* cp = malloc (sizeof(creadorPrefijo_t));
	if (!cp) return NULL;

	cp->cantidad = b;
	cp->listas = malloc (sizeof(lista_t*) * b);
	for (size_t i = 0; i < b; i++){
		cp->listas[i] = lista_crear();
	}

	cp->cero = malloc (sizeof(size_t));
	*(cp->cero) = 0;

	cp->uno = malloc(sizeof(size_t));
	*(cp->uno) = 1;


	heap_t* heap = heap_crear(comparacionClasesEquivalencia);
	for (size_t i = 0; i < b; i++){
		size_t* valor = malloc (sizeof(size_t));
		*valor = i;
		lista_t* aux = lista_crear();
		lista_insertar_primero(aux, valor);
		heap_encolar(heap, aux);
	}

	while (heap_cantidad(heap) > 1){
		lista_t* menor = heap_desencolar(heap);
		lista_t* mayor = heap_desencolar(heap);

		lista_t* aux = lista_crear();
		vaciarYAgregar(menor, cp->listas, cp->cero, aux);
		vaciarYAgregar(mayor, cp->listas, cp->uno, aux);
		lista_destruir(menor, NULL);
		lista_destruir(mayor, NULL);
		heap_encolar(heap, aux);
	}

	lista_t* final = heap_desencolar(heap);
	lista_destruir(final, free);
	heap_destruir(heap, NULL);

	return cp;
}

lista_t* copiaDeLista(lista_t* aux){
	lista_t* copy = lista_crear();
	lista_iter_t* iter = lista_iter_crear(aux);
	while (!lista_iter_al_final(iter)){
		size_t* valor = lista_iter_ver_actual(iter);
		size_t* valor_cpy = malloc (sizeof(size_t));
		*valor_cpy = *valor;
		lista_insertar_ultimo(copy, valor_cpy);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return copy;
}


lista_t* creadorPrefijo_obtener_lista_bits(creadorPrefijo_t *cp,unsigned int num){
	if (!cp) return NULL;
	if (cp->cantidad <= num) return NULL;

	return copiaDeLista(cp->listas[num]);
}

void creadorPrefijo_destruir(creadorPrefijo_t *cp){
	if (!cp) return;
	for (size_t i = 0; i < cp->cantidad; i++){
		lista_destruir(cp->listas[i], NULL);
	}
	free(cp->listas);
	free(cp->uno);
	free(cp->cero);
	free(cp);
}

bool masDeUnoPosible(bool* posibles, size_t cant, unsigned int* rta){
	bool posible = false;
	bool primero = true;

	for (size_t i = 0; i < cant && !posible; i++){
		if (posibles[i]){
			if (primero){
				primero = false;
				*rta = i;
			}else{
				posible = true;
			}
		}
	}
	return posible;
}

unsigned int creadorPrefijo_obtener_numero(creadorPrefijo_t* cp,debuffer_t* debuffer){
	if (!cp) return 0;
	size_t b = cp->cantidad;
	bool posibles[b];
	lista_iter_t* iteradores[b];
	for (size_t i = 0; i < b; i++){
		posibles[i] = true;
		iteradores[i] = lista_iter_crear(cp->listas[i]);
	}

	unsigned int rta;

	while (masDeUnoPosible(posibles, b, &rta)){
		Byte_t bit = debuffer_leer_bit(debuffer);
		for (size_t i = 0; i < b; i++){
			if (!posibles[i]) continue;
			size_t* valor = lista_iter_ver_actual(iteradores[i]);
			if ((*valor) == bit){
				lista_iter_avanzar(iteradores[i]);
			}else{
				posibles[i] = false;
				lista_iter_destruir(iteradores[i]);
			}
		}
	}
	lista_iter_destruir(iteradores[rta]);
	return rta;
}

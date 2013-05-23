#include "buscador.h"
#include "../Carpetas Compartidas/TDAs/trie.h"
#include "../Carpetas Compartidas/Manejo de Archivos/termino.h"
#include "Lexico/levantador.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//Por ahora solo podemos tenemos la estructura donde guardamos el lexico, pero podriamos
//necesitar varias cosas mas!
struct buscador{
	trie_t* almacenador;
};


void destructor_terminos(void* dato){
	termino_t* termino = (termino_t*) dato;
	termino_destruir(termino);
}

bool guardar_aux(void* a, const char* b, void* c){
	return trie_guardar((trie_t*) a, b, c);
}

buscador_t* buscador_crear(const char* rutaFrontCoding, const char* rutaDiferentes){
	buscador_t* b = malloc (sizeof(buscador_t));
	if (!b) return NULL;

	b->almacenador = trie_crear(destructor_terminos);
	levantador_obtenerContenedorLexico(b->almacenador, guardar_aux, rutaFrontCoding, rutaDiferentes);

	return b;
}

void buscador_buscar(buscador_t* buscador, lista_t* terminos_buscados){
	if (!buscador || !terminos_buscados || lista_largo(terminos_buscados) == 0){
		printf("No hay buscador o no hay terminos a buscar\n");
		return;
	}

	lista_iter_t* iter = lista_iter_crear(terminos_buscados);

	while (!lista_iter_al_final(iter)){
		//Esto es lo que hay que cambiar:
		char* termino = lista_iter_ver_actual(iter);
		printf("%s: ", termino);
		if (trie_pertenece(buscador->almacenador, termino))
			printf("OK!\n");
		else
			printf("NO ESTA!\n");

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
}


void buscador_destruir(buscador_t* busq){
	if (!busq) return;
	trie_destruir(busq->almacenador);
	free(busq);
}

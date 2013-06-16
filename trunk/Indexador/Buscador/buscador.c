#include "buscador.h"
#include "../Carpetas Compartidas/TDAs/hash.h"
#include "../Carpetas Compartidas/TDAs/heap.h"
#include "../Carpetas Compartidas/Manejo de Archivos/termino.h"
#include "../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "Lexico/levantador.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define IMPORTANCIA_MINIMA 0.6
#define MAX_CANT_QUERY_SIMILAR 15
#define MAX_SOLUCIONES_CALIDAD 5
#define MONTECARLO_INTENTOS 20
#include <time.h>

/**************************************************************************************/
/*                                     BUSCADOR                                       */
/**************************************************************************************/

//Por ahora solo podemos tenemos la estructura donde guardamos el lexico, pero podriamos
//necesitar varias cosas mas!
struct buscador{
	hash_t* almacenador;
	size_t cant_docs;
};


void destructor_terminos(void* dato){
	termino_t* termino = (termino_t*) dato;
	termino_destruir(termino);
}

bool guardar_aux(void* a, const char* b, void* c){
	return hash_guardar((hash_t*) a, b, c);
}

buscador_t* buscador_crear(const char* rutaFrontCoding, const char* rutaDiferentes, size_t cant){
	buscador_t* b = malloc (sizeof(buscador_t));
	if (!b) return NULL;
	termino_setearCantDocs(cant);
	b->almacenador = hash_crear(destructor_terminos);
	levantador_obtenerContenedorLexico(b->almacenador, guardar_aux, rutaFrontCoding, rutaDiferentes);
	b->cant_docs = cant;
	return b;
}

resultado_t* buscador_buscar(buscador_t* buscador, lista_t* terminos_buscados, const char* dirOffsets, const char* ruta_tams){
	if (!buscador || !terminos_buscados || lista_largo(terminos_buscados) == 0){
		return NULL;
	}

	termino_t** vector_terminos = malloc (sizeof(termino_t*) * lista_largo(terminos_buscados));
	if (!vector_terminos) return NULL;
	size_t cont = 0;

	lista_iter_t* iter = lista_iter_crear(terminos_buscados);
	while (!lista_iter_al_final(iter)){
		//Esto es lo que hay que cambiar:
		char* termino = lista_iter_ver_actual(iter);

		if (hash_pertenece(buscador->almacenador, termino)){
			termino_t* term = hash_obtener(buscador->almacenador, termino);
			vector_terminos[cont] = term;
		}else{
			vector_terminos[cont] = NULL; 	//Aca podriamos decir que no hay ninguna solucion
		}
		cont++;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);

	resultado_t* resul = resultado_crear(vector_terminos, cont, dirOffsets, ruta_tams);
	free(vector_terminos);
	return resul;
}


void buscador_destruir(buscador_t* busq){
	if (!busq) return;
	hash_destruir(busq->almacenador);
	free(busq);
}

void buscador_busquedaPuntual(buscador_t* buscador, const char* termino, const char* index, const char* paths, const char* offsetPaths, const char* ruta_tams){
	if (!buscador || !termino) return;
	printf("Busqueda por palabra: %s\n",termino);

	termino_t* term = hash_obtener(buscador->almacenador, termino);
	if (!term){
		printf("La palabra buscada no aparece en ningun documento\n");
		return;
	}

	lista_t* infoTermino = termino_decodificarPunteros(term, index, ruta_tams);

	lista_t* documentos = lista_borrar_primero(infoTermino);
	lista_iter_t* iter = lista_iter_crear(documentos);
	printf("El termino se encuentra en los documentos:\n");
	while (!lista_iter_al_final(iter)){
		size_t doc = *((size_t*) (lista_iter_ver_actual(iter)));
		char* nomDoc = __obtenerNombreDoc(paths, offsetPaths, doc);
		printf("%s ", nomDoc);
		free(nomDoc);
		lista_t* posiciones = lista_borrar_primero(infoTermino);

		printf("%u veces\n",lista_largo(posiciones));

		lista_destruir(posiciones, free);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	lista_destruir(documentos, free);
	lista_destruir(infoTermino, NULL);

}

/**********************************************************************************/
/*                            Buscador Incompleto                                 */
/**********************************************************************************/

typedef struct{
	lista_t* terminos_reales;
	float calidad;
	lista_t* solucion;
}conjunto_calidad_t;


conjunto_calidad_t* conjunto_calidad_crear(lista_t* terminos, float imp, lista_t* sol){
	conjunto_calidad_t* conj = malloc (sizeof(conjunto_calidad_t));
	conj->calidad = imp;
	conj->terminos_reales = terminos;
	conj->solucion = sol;
	return conj;
}

void conjunto_calidad_destruir(conjunto_calidad_t* conj){
	if (!conj) return;
	lista_destruir(conj->solucion, destructor_solucion);
	lista_destruir(conj->terminos_reales, NULL);
	free(conj);
}

void conjunto_calidad_imprimir(conjunto_calidad_t* conj, const char* paths, const char* offsets){
	if (!conj) return;
	printf("Para una busqueda de calidad %f: ", conj->calidad);
	lista_iter_t* iter = lista_iter_crear(conj->terminos_reales);
	while (!lista_iter_al_final(iter)){
		termino_t* term = lista_iter_ver_actual(iter);
		char* word = termino_obtenerPalabra(term);
		printf ("%s ", word);
		free(word);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	printf("se encontro:\n");
	solucion_emitir(conj->solucion, paths, offsets);
	printf("\n");
}

int comparacionConjuntoCalidad(const void* a, const void* b){
	conjunto_calidad_t* conj1 = (conjunto_calidad_t*) a;
	conjunto_calidad_t* conj2 = (conjunto_calidad_t*) b;
	if (conj1->calidad > conj2->calidad) return 1;
	if (conj2->calidad > conj1->calidad) return -1;
	return 0;
}

lista_t* copiarSinAlguno(lista_t* org, size_t salteo){
	lista_iter_t* iter = lista_iter_crear(org);
	lista_t* cpy = lista_crear();
	size_t i = 0;
	while (!lista_iter_al_final(iter)){
		if (i != salteo){
			lista_insertar_ultimo(cpy, lista_iter_ver_actual(iter));
		}
		i++;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return cpy;
}

bool solucionador_Backtracking(lista_t* lista_actual, lista_t* org, heap_t* heap, const char* dirOffsets, const char* rutaTams){
	float imp = calculoImportancia (lista_actual, org);
	if (imp < IMPORTANCIA_MINIMA)
		return false;

	termino_t* terminos[lista_largo(lista_actual)];
	lista_iter_t* iter = lista_iter_crear(lista_actual);
	size_t idx = 0;
	while (!lista_iter_al_final(iter)){
		terminos[idx] = lista_iter_ver_actual(iter);
		lista_iter_avanzar(iter);
		idx++;
	}
	lista_iter_destruir(iter);

	resultado_t* resultado = resultado_crear(terminos,lista_largo(lista_actual),dirOffsets,rutaTams);
	lista_t* soluciones = resultado_realizarIntersecciones(resultado);
	resultado_destruir(resultado);
	if (lista_largo(soluciones) > 0){
		conjunto_calidad_t* conj = conjunto_calidad_crear(lista_actual,imp,soluciones);
		heap_encolar(heap, conj);
		return true;
	}
	lista_destruir(soluciones, destructor_solucion);

	if (lista_largo(lista_actual) == 2) return false;

	for (size_t i = 0; i < lista_largo(lista_actual) && (heap_cantidad(heap) < MAX_SOLUCIONES_CALIDAD); i++){
		lista_t* aux = copiarSinAlguno(lista_actual, i);
		if (!solucionador_Backtracking(aux, org, heap, dirOffsets, rutaTams))
			lista_destruir(aux, NULL);
	}

	return false;
}


void buscador_busquedaImperfecta(buscador_t* buscador, lista_t* query, const char* index, const char* paths, const char* offsetPaths, const char* ruta_tams){
	if (lista_largo(query) > MAX_CANT_QUERY_SIMILAR ){
		printf("No se realizan busquedas similares pues la cantidad de terminos de la consulta es muy grande, lo que ocasionaria una busqueda demasiado lenta\n");
		return;
	}
	if (lista_largo(query) <= 2) return;
	printf("Se realizan busquedas similares!\n");
	printf("Aguarde un momento mientras se van realizando los calculos (este proceso puede tardar algunos minutos, pues es mas complejo que una busqueda normal)\n");


	lista_t* lista_terminos = lista_crear();
	lista_iter_t* iter = lista_iter_crear(query);
	while (!lista_iter_al_final(iter)){
		lista_insertar_ultimo(lista_terminos, hash_obtener(buscador->almacenador, lista_iter_ver_actual(iter)));
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);

	heap_t* heap = heap_crear(comparacionConjuntoCalidad);

	for (size_t i = 0; i < lista_largo(lista_terminos) && heap_cantidad(heap) < MAX_SOLUCIONES_CALIDAD; i++){
		lista_t* aux = copiarSinAlguno(lista_terminos, i);
		if (!solucionador_Backtracking(aux, lista_terminos, heap, index, ruta_tams))
			lista_destruir(aux, NULL);
	}


	if (heap_cantidad(heap) == 0){
		printf("No se encontraron soluciones similares de calidad minima %f\n", IMPORTANCIA_MINIMA);
		heap_destruir(heap, NULL);
		return;
	}

	while (!heap_esta_vacio(heap)){
		conjunto_calidad_t* conj = heap_desencolar(heap);
		conjunto_calidad_imprimir(conj, paths, offsetPaths);
		conjunto_calidad_destruir(conj);
	}

	heap_destruir(heap,NULL);
}



void buscador_busquedaImperfectaMontecarlo(buscador_t* buscador, lista_t* query, const char* index, const char* paths, const char* offsetPaths, const char* ruta_tams){
	if (lista_largo(query) <= 2) return;
	printf("Se realizan busquedas similares via Montecarlo!\n");
	printf("Aguarde un momento mientras se van realizando los calculos (este proceso puede tardar algunos minutos, pues es mas complejo que una busqueda normal)\n");

	size_t intentos = 0;
	bool solucionado = false;

	lista_t* terminosDeLaQuery = lista_crear();
	lista_iter_t* iter = lista_iter_crear(query);
	while (!lista_iter_al_final(iter)){
		char* nom = lista_iter_ver_actual(iter);
		lista_insertar_ultimo(terminosDeLaQuery, hash_obtener(buscador->almacenador, nom));
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);

	lista_t* copy_out = copiarSinAlguno(terminosDeLaQuery, lista_largo(query)+1);
	conjunto_calidad_t* conj = NULL;
	while (intentos < MONTECARLO_INTENTOS && !solucionado && lista_largo(copy_out) > 2){
		size_t eliminar = (rand() % lista_largo(copy_out));
		lista_t* aux = copiarSinAlguno(copy_out, eliminar);
		lista_destruir(copy_out, NULL);
		copy_out = aux;

		termino_t* terminos[lista_largo(copy_out)];
		lista_iter_t* iter = lista_iter_crear(copy_out);
		size_t idx = 0;
		while (!lista_iter_al_final(iter)){
			terminos[idx] = lista_iter_ver_actual(iter);
			lista_iter_avanzar(iter);
			idx++;
		}
		lista_iter_destruir(iter);

		resultado_t* resultado = resultado_crear(terminos,lista_largo(copy_out),index,ruta_tams);
		lista_t* soluciones = resultado_realizarIntersecciones(resultado);
		resultado_destruir(resultado);
		if (lista_largo(soluciones) > 0){
			float imp = calculoImportancia (copy_out, terminosDeLaQuery);
			conj = conjunto_calidad_crear(copy_out, imp, soluciones);
			solucionado = true;
		}else{
			lista_destruir(soluciones, destructor_solucion);
		}
		intentos++;
	}
	lista_destruir(terminosDeLaQuery,NULL);
	if (!solucionado){
		printf("No se encontraron resultados via le metodo de Montecarlo\n");
		lista_destruir(copy_out, NULL);
		return;
	}

	conjunto_calidad_imprimir(conj, paths, offsetPaths);
	conjunto_calidad_destruir(conj);

}

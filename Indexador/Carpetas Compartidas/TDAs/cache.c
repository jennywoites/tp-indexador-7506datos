#include "cache.h"
#include "heap.h"
#include "hash.h"
#include <stdlib.h>
#include <stddef.h>

#define CACHE_MAX_TAM 1000

struct cache{
	heap_t* cola_salida;
	hash_t* almacenador;
	cache_generar_copia_t copiador;
	cache_obtener_clave_t claves;
	cache_comparacion_t compare;
	cache_destruccion_t destruct;
};



cache_t* cache_crear(cache_generar_copia_t copia, cache_comparacion_t comp, cache_obtener_clave_t claveador,cache_destruccion_t destruct){
	if (!copia || !comp || !claveador) return NULL;

	cache_t* cache = malloc (sizeof(cache_t));
	if (!cache) return NULL;
	cache->copiador = copia;
	cache->claves = claveador;
	cache->compare = comp;
	cache->destruct = destruct;
	cache->cola_salida = heap_crear(comp);
	if (!cache->cola_salida){
		free(cache);
		return NULL;
	}
	cache->almacenador = hash_crear(destruct);
	if (!cache->almacenador){
		heap_destruir(cache->cola_salida, NULL);
		free(cache);
		return NULL;
	}

	return cache;
}

void cache_destruir(cache_t* cache){
	if (!cache) return;

	hash_destruir(cache->almacenador);
	heap_destruir(cache->cola_salida, NULL);
	free(cache);
}

bool cache_pertenece(cache_t* cache, const char* clave){
	if (!cache) return false;

	return hash_pertenece(cache->almacenador, clave);
}

void* cache_obtener(cache_t* cache, const char* clave){
	if (!cache_pertenece(cache, clave)) return NULL;

	void* dato = hash_obtener(cache->almacenador, clave);
	void* cpy = cache->copiador(dato);
	return cpy;
}

bool cache_guardar(cache_t* cache, const char* clave, void* dato){
	if (!cache) return false;

	if (hash_cantidad(cache->almacenador) < CACHE_MAX_TAM){
		heap_encolar(cache->cola_salida, dato);
		hash_guardar(cache->almacenador, clave, dato);
		return true;
	}

	void* dato_salida = heap_ver_max(cache->cola_salida);
	if (cache->compare(dato_salida, dato) <= 0){
		return false;
	}

	char* clave_salida = cache->claves(heap_desencolar(cache->cola_salida));
	cache->destruct(hash_borrar(cache->almacenador,clave_salida));
	hash_guardar(cache->almacenador, clave, dato);
	heap_encolar(cache->cola_salida, dato);
	return true;
}

#ifndef CACHE_H_
#define CACHE_H_
#include <stdbool.h>

typedef struct cache cache_t;

typedef void* (*cache_generar_copia_t) (const void*);
typedef int (*cache_comparacion_t) (const void*,const void*);
typedef void (*cache_destruccion_t) (void*);
typedef char* (*cache_obtener_clave_t) (void*);


cache_t* cache_crear(cache_generar_copia_t copia, cache_comparacion_t comp, cache_obtener_clave_t claveador,cache_destruccion_t destruct);

void cache_destruir(cache_t* cache);

bool cache_pertenece(cache_t* cache, const char* clave);

void* cache_obtener(cache_t* cache, const char* clave);

bool cache_guardar(cache_t* cache, const char* clave, void* dato);



#endif /* CACHE_H_ */

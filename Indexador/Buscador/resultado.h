#ifndef RESULTADO_H_
#define RESULTADO_H_
#include "../Carpetas Compartidas/Manejo de Archivos/termino.h"

typedef struct resultado resultado_t;

resultado_t* resultado_crear(termino_t** terminos, size_t cantidad, const char* dirOffsets, const char* ruta_tams);

lista_t* resultado_realizarIntersecciones(resultado_t* resul);

void solucion_emitir(lista_t* soluciones, const char* paths, const char* offsets);

void resultado_destruir(resultado_t* resul);

void destructor_solucion(void* a);

#endif /* RESULTADO_H_ */

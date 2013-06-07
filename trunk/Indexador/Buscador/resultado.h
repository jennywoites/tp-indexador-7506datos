#ifndef RESULTADO_H_
#define RESULTADO_H_
#include "../Carpetas Compartidas/Manejo de Archivos/termino.h"

typedef struct resultado resultado_t;

resultado_t* resultado_crear(termino_t** terminos, size_t cant, const char* dirOffsets);

void resultado_emitirListado(resultado_t* resultado, lista_t* query, const char* paths, const char* offsets);

void resultado_destruir(resultado_t* resul);


#endif /* RESULTADO_H_ */

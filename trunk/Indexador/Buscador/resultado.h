#ifndef RESULTADO_H_
#define RESULTADO_H_
#include "../Carpetas Compartidas/Manejo de Archivos/termino.h"

typedef struct resultado resultado_t;

resultado_t* resultado_crear(termino_t** terminos, size_t cant, const char* dirOffsets);


#endif /* RESULTADO_H_ */

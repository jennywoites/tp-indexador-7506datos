#ifndef BUSCADOR_H_
#define BUSCADOR_H_
#include "../Carpetas Compartidas/TDAs/lista.h"
typedef struct buscador buscador_t;



buscador_t* buscador_crear(const char* rutaFrontCoding, const char* rutaDiferentes);

void buscador_destruir(buscador_t*);

//FIXME: esto deberia devolver un solucion_t*, por ahora solo va a imprimir los OK :P
void buscador_buscar(buscador_t* buscador, lista_t* terminos_buscados);



#endif /* BUSCADOR_H_ */

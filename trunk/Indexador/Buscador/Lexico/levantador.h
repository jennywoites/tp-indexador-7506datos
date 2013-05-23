#ifndef LEVANTADOR_H_
#define LEVANTADOR_H_
#define LEVANTADOR_OK 0
#define LEVANTADOR_ERROR 1

#include <stdbool.h>

typedef bool (*contenedor_guardar_dato_t)(void *, const char* , void*);


/* Funcion que permite obtener un trie con el lexico a partir
 * del archivo de front coding + los diferentes*/
int levantador_obtenerContenedorLexico(void* contenedor,contenedor_guardar_dato_t funcion ,const char* rutaFrontCoding, const char* rutaDiferentes);



#endif /* LEVANTADOR_H_ */

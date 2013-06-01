#ifndef DEBUFFER_H_
#define DEBUFFER_H_

#define DEBUFFER_EOF 1
#define DEBUFFER_OK 0
#define DEBUFFER_ERROR 2
#include <stdio.h>
#include <stddef.h>
#include "../Codigos/matematicaEspecial.h"

typedef struct debuffer debuffer_t;

debuffer_t* debuffer_crear(FILE* fd, size_t cant_bytes);

Byte_t debuffer_leer_bit(debuffer_t* debuff);

void debuffer_descartar_bits(debuffer_t* debuff, size_t bits_a_desechar){

void debuffer_destruir(debuffer_t* debuff);

#endif /* DEBUFFER_H_ */

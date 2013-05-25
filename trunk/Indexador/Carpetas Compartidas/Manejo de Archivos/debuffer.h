#ifndef DEBUFFER_H_
#define DEBUFFER_H_

#include <stdio.h>
#include "../Codigos/matematicaEspecial.h"

typedef struct debuffer debuffer_t;

debuffer_t* debuffer_crear(FILE* fd);

Byte_t debuffer_leer_bit(debuffer_t* debuff);

void debuffer_destruir(debuffer_t* debuff);

#endif /* DEBUFFER_H_ */

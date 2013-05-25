#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdio.h>
#include "../Codigos/matematicaEspecial.h"

typedef struct buffer buffer_t;

buffer_t* buffer_crear(FILE* fd);

void buffer_escribir_bit(buffer_t* buff, Byte_t num);

void buffer_escribir_LS_bits(buffer_t* buff, Byte_t num, size_t longitud );

void buffer_escribir_MS_bits(buffer_t* buff, Byte_t num, size_t longitud );

void buffer_escribir_byte(buffer_t* buff, Byte_t num);

void buffer_escribir_bytes(buffer_t* buff, Byte_t* num, size_t cant_bytes);

void buffer_destruir(buffer_t* buff);


#endif /* BUFFER_H_ */

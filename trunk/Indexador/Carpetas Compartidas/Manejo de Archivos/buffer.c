#include "buffer.h"
#include <stddef.h>
#include <stdlib.h>

struct buffer{
	FILE* arch_salida;
	size_t cant_bits_almacenados;
	Byte_t* bits_almacenados;
	size_t contador_bits;
};


buffer_t* buffer_crear(FILE* fd){
	buffer_t* buff = malloc(sizeof(buffer_t) );
	buff->bits_almacenados = malloc(8* sizeof(Byte_t) );
	buff->cant_bits_almacenados = 0;
	buff->arch_salida = fd;
	buff->contador_bits = 0;

	return buff;
}

Byte_t buffer_crear_byte(buffer_t* buff){
	if (buff->cant_bits_almacenados!=8)
		return 0;
	Byte_t byte_completo = 0;
	for (size_t i = 0; i < 8; i++){
		byte_completo += Byte_dosElevadoALa(i) * buff->bits_almacenados[i] ;
	}
	return byte_completo;
}

void buffer_escribir_a_archivo(buffer_t* buff){
	Byte_t byte_completo;
	byte_completo = buffer_crear_byte(buff);

	if(buff->arch_salida)
		fwrite(&byte_completo, 1, 1, buff->arch_salida);
		//para pruebas:
		//fprintf(buff->arch_salida, "%d", byte_completo);

	buff->cant_bits_almacenados=0;
}

void buffer_escribir_bytes(buffer_t* buff, Byte_t* num, size_t cant_bytes){
	for (size_t i=0; i<cant_bytes ;i++)
		buffer_escribir_byte(buff, num[i] );
}

void buffer_escribir_byte(buffer_t* buff, Byte_t num){
	size_t longitud = 8;
	buffer_escribir_LS_bits(buff, num, longitud);
}

void buffer_escribir_LS_bits(buffer_t* buff, Byte_t num, size_t longitud ){
	for (int i=longitud-1; i>=0 ;i--){
		if ( num >= dosElevadoALa(i) ){
			num = num - dosElevadoALa(i);
			buffer_escribir_bit(buff, 1);
		}else{
			buffer_escribir_bit(buff, 0);
		}
	}
}

void buffer_escribir_MS_bits(buffer_t* buff, Byte_t num, size_t longitud ){
	for (int i= 7; i>7-longitud ;i--){
		if ( num >= dosElevadoALa(i) ){
			num = num - dosElevadoALa(i);
			buffer_escribir_bit(buff, 1);
		}else{
			buffer_escribir_bit(buff, 0);
		}
	}
}

void buffer_escribir_bit(buffer_t* buff, Byte_t num){
	(buff->bits_almacenados) [ 8 - 1 - buff->cant_bits_almacenados ] = num ;

	buff->cant_bits_almacenados++;
	
	buff->contador_bits++;

	if (buff->cant_bits_almacenados==8)
		buffer_escribir_a_archivo(buff);
}

size_t buffer_obtener_contador(buffer_t* buff){
	return buff->contador_bits;
}

void buffer_reset_contador(buffer_t* buff){
	buff->contador_bits = 0;
}

void buffer_rellenar(buffer_t* buff){
	Byte_t uno = 1;
	if( buff->cant_bits_almacenados == 0)
		return ;
	for(int i = 0; i <= 7 /*- buff->cant_bits_almacenados*/; i++ ){
		buffer_escribir_bit(buff,uno);
	}
}

void buffer_destruir(buffer_t* buff){
	free(buff->bits_almacenados);
	free(buff);
}

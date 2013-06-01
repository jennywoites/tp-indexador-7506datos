#include "debuffer.h"
#include <stddef.h>
#include <stdlib.h>

struct debuffer{
	FILE* arch_entrada;
	size_t cant_bits_almacenados;
	Byte_t* bits_almacenados;
	size_t cant_bytes;
	size_t cant_bytes_leidos;

};

debuffer_t* debuffer_crear(FILE* fd, size_t cant_bytes){
	debuffer_t* debuff = malloc(sizeof(debuffer_t) );
	debuff->bits_almacenados = malloc(8* sizeof(Byte_t) );
	debuff->cant_bits_almacenados = 0;
	debuff->arch_entrada = fd;
	debuff->cant_bytes = cant_bytes;
	debuff->cant_bytes_leidos = 0;
	
	return debuff;
}

void debuffer_escribir_bit(debuffer_t* debuff, Byte_t num){
	(debuff->bits_almacenados) [ 8 - 1 - debuff->cant_bits_almacenados ] = num ;

	debuff->cant_bits_almacenados++;
}

void guardar_byte_en_debuffer(debuffer_t* debuff,Byte_t byte_completo ){
	unsigned int pot2i;
	for(int i = 8 - 1; i>=0; i-- ){
		pot2i = dosElevadoALa(i);
		if (byte_completo >= pot2i ){
			byte_completo = byte_completo - pot2i;
			debuffer_escribir_bit(debuff,1);
		}else
			debuffer_escribir_bit(debuff,0);
	}
}

size_t debuffer_leer_de_archivo(debuffer_t* debuff){
	if ( debuff->cant_bytes_leidos == debuff->cant_bytes )
		return DEBUFFER_EOF;
	Byte_t byte_completo;
	fread(&byte_completo, 1, 1, debuff->arch_entrada);
	guardar_byte_en_debuffer(debuff, byte_completo);
	debuff->cant_bits_almacenados = 8;
	debuff->cant_bytes_leidos++;
	return DEBUFFER_OK;
}

void debuffer_descartar_bits(debuffer_t* debuff, size_t bits_a_desechar){
	//FIXME: Eze: No recuerdo bien si el de leer bit solo avanza en el debuffer o no. Hay que probarlo
	for(int i =0; i<bits_a_desechar; i++){
		debuffer_leer_bit(debuff);
	}
}

Byte_t debuffer_leer_bit(debuffer_t* debuff){
	if (debuff->cant_bits_almacenados==0){
		size_t estado = debuffer_leer_de_archivo(debuff);
		if (estado == DEBUFFER_EOF)
			return DEBUFFER_ERROR;
	}

	Byte_t num = (debuff->bits_almacenados) [ debuff->cant_bits_almacenados -1] ;

	debuff->cant_bits_almacenados--;

	return num;

}


void debuffer_destruir(debuffer_t* debuff){
	free(debuff->bits_almacenados);
	free(debuff);
}

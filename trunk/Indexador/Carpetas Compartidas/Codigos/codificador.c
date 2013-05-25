#include "codificador.h"
#include <stddef.h>
#include "matematicaEspecial.h"
#include <stdio.h>

struct codificador{
	buffer_t* buffer;
};

codificador_t* codificador_crear(buffer_t* buffer){
	codificador_t* cod = malloc (sizeof(codificador_t));
	if (!cod)
		return NULL;
	
	cod->buffer = buffer;
	
	return cod;
}

void codificador_codificarUnario(codificador_t* cod, unsigned int num){
	if(num<=0)
		return ;
	while(num>1){
		num--;
		buffer_escribir_bit(cod->buffer,1);
	}
	buffer_escribir_bit(cod->buffer,0);
}

void codificador_codificarBinario(codificador_t* cod, unsigned int num, size_t longitud){
	if (longitud <= 0)
		return ;
	unsigned int pot2i;
	for(int i = longitud - 1; i>=0; i-- ){
		pot2i = dosElevadoALa(i);
		if (num >= pot2i ){
			num = num - pot2i;
			buffer_escribir_bit(cod->buffer,1);
		}else
			buffer_escribir_bit(cod->buffer,0);
	}
}


void codificador_codificarGamma(codificador_t* cod, unsigned int num){
	if(num<=0)
		return ;
	//Obtengo los numeros que tengo que escribir
	unsigned int piso_log = maxPot2(num);
	unsigned int num_unario = 1 + piso_log;
	unsigned int num_binario = num - dosElevadoALa(piso_log);
	size_t cant_bits_binario = piso_log;
	
	codificador_codificarUnario(cod, num_unario);
	codificador_codificarBinario(cod, num_binario, cant_bits_binario);
}

void codificador_codificarDelta(codificador_t* cod, unsigned int num){
	if(num<=0)
		return ;
	//Obtengo los numeros que tengo que escribir
	unsigned int piso_log = maxPot2(num);
	unsigned int num_gamma = 1 + piso_log;
	unsigned int num_binario = num - dosElevadoALa(piso_log);
	size_t cant_bits_binario = piso_log;
	
	codificador_codificarGamma(cod, num_gamma);
	codificador_codificarBinario(cod, num_binario, cant_bits_binario);
}

void codificador_destruir(codificador_t* cod){
	free(cod);
}

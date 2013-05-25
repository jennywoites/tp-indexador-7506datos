#include "decodificador.h"
#include <stddef.h>
#include "matematicaEspecial.h"
#include <stdio.h>

struct decodificador{
	debuffer_t* debuffer;
};

decodificador_t* decodificador_crear(debuffer_t* debuffer){
	decodificador_t* decod = malloc (sizeof(decodificador_t));
	if (!decod)
		return NULL;
	
	decod->debuffer = debuffer;
	
	return decod;
}

unsigned int decodificador_decodificarUnario(decodificador_t* decodificador){
	if (!decodificador)
		return NO_NUMERO;
	
	unsigned int cant_bits = 1;
	Byte_t bit = debuffer_leer_bit(decodificador->debuffer);
	
	while(bit != 0){
		cant_bits ++;
		bit = debuffer_leer_bit(decodificador->debuffer);
	}
	
	unsigned int numero = cant_bits;
	return numero;
}

unsigned int decodificador_decodificarBinario(decodificador_t* decodificador, size_t longitud){
	if (!decodificador || (longitud == 0))
		return NO_NUMERO;

	unsigned int numero = 0;
	Byte_t bit;
	
	for(int i = longitud-1; i>=0; i--){
		bit = debuffer_leer_bit(decodificador->debuffer);
		
		if (bit == 1)
			numero = numero + dosElevadoALa(i);
	}
	
	return numero;
}

unsigned int decodificador_decodificarGamma(decodificador_t* decodificador){
	if (!decodificador)
		return NO_NUMERO;

	unsigned int numero = 0;
	unsigned int numero_unario = decodificador_decodificarUnario(decodificador);
	size_t cant_bits = numero_unario - 1;
	unsigned int numero_binario = decodificador_decodificarBinario(decodificador, cant_bits);
	
	numero = numero_binario + dosElevadoALa(cant_bits);
	
	return numero;
}

unsigned int decodificador_decodificarDelta(decodificador_t* decodificador){
	if (!decodificador)
		return NO_NUMERO;

	unsigned int numero = 0;
	unsigned int numero_gamma = decodificador_decodificarGamma(decodificador);
	size_t cant_bits = numero_gamma - 1;
	unsigned int numero_binario = decodificador_decodificarBinario(decodificador, cant_bits);
	
	numero = numero_binario + dosElevadoALa(cant_bits);
	
	return numero;
}

void decodificador_destruir(decodificador_t* decodificador){
	free(decodificador);
}
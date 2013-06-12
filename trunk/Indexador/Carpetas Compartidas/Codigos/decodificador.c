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

unsigned int decodificador_decodificarUnario(debuffer_t* decodificador){
	if (!decodificador)
		return NO_NUMERO;
	
	unsigned int cant_bits = 1;
	Byte_t bit = debuffer_leer_bit(decodificador);
	if (bit == DEBUFFER_ERROR)
		return NO_NUMERO;
	
	while(bit != 0){
		cant_bits ++;
		bit = debuffer_leer_bit(decodificador);
		if (bit == DEBUFFER_ERROR)
			return NO_NUMERO;
	}
	
	unsigned int numero = cant_bits;
	return numero;
}

unsigned int decodificador_decodificarBinario(debuffer_t* decodificador, size_t longitud){
	if (!decodificador || (longitud == 0))
		return NO_NUMERO;

	unsigned int numero = 0;
	Byte_t bit;
	
	for(int i = longitud-1; i>=0; i--){
		bit = debuffer_leer_bit(decodificador);
		if (bit == DEBUFFER_ERROR)
			return NO_NUMERO;
		
		if (bit == 1)
			numero = numero + dosElevadoALa(i);
	}
	
	return numero;
}

unsigned int decodificador_decodificarGamma(debuffer_t* decodificador){
	if (!decodificador)
		return NO_NUMERO;

	unsigned int numero = 0;
	unsigned int numero_unario = decodificador_decodificarUnario(decodificador);
	size_t cant_bits = numero_unario - 1;
	if (numero_unario == NO_NUMERO)
			return NO_NUMERO;
	unsigned int numero_binario = decodificador_decodificarBinario(decodificador, cant_bits);
	
	numero = numero_binario + dosElevadoALa(cant_bits);
	
	return numero;
}

unsigned int decodificador_decodificarDelta(debuffer_t* decodificador){
	if (!decodificador)
		return NO_NUMERO;

	unsigned int numero = 0;
	unsigned int numero_gamma = decodificador_decodificarGamma(decodificador);
	if (numero_gamma == NO_NUMERO)
		return NO_NUMERO;
	size_t cant_bits = numero_gamma - 1;
	unsigned int numero_binario = decodificador_decodificarBinario(decodificador, cant_bits);
	
	numero = numero_binario + dosElevadoALa(cant_bits);
	
	return numero;
}

unsigned int decodificador_decodificarGolomb(debuffer_t* debuffer, size_t b, arbol_huff_t* arbol){
	if ((!debuffer) || (b == 0))
			return NO_NUMERO;

	if(!arbol)
		return NO_NUMERO;

	unsigned int numero = 0;
	unsigned int qmas1 = decodificador_decodificarUnario(debuffer);
	unsigned int r = arbol_huff_obtener_numero(arbol,debuffer);

	unsigned int q = qmas1 - 1;

	numero = r + 1 + (q * b);

	return numero;
}

void decodificador_destruir(decodificador_t* decodificador){
	free(decodificador);
}

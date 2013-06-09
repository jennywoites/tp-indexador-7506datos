#include "codificador.h"
#include <stddef.h>
#include "matematicaEspecial.h"
#include <stdio.h>
#include "../TDAs/abb.h"
#include "../TDAs/arbolHuff.h"
#include "../TDAs/lista.h"
#include <string.h>
#include "../TDAs/heap.h"

#define CANT_CHARS 3

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

void codificador_codificarUnario(buffer_t* cod, unsigned int num){
	if(num<=0)
		return ;
	while(num>1){
		num--;
		buffer_escribir_bit(cod,1);
	}
	buffer_escribir_bit(cod,0);
}

void codificador_codificarBinario(buffer_t* cod, unsigned int num, size_t longitud){
	if (longitud <= 0)
		return ;
	unsigned int pot2i;
	for(int i = longitud - 1; i>=0; i-- ){
		pot2i = dosElevadoALa(i);
		if (num >= pot2i ){
			num = num - pot2i;
			buffer_escribir_bit(cod,1);
		}else
			buffer_escribir_bit(cod,0);
	}
}


void codificador_codificarGamma(buffer_t* cod, unsigned int num){
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

void codificador_codificarDelta(buffer_t* cod, unsigned int num){
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

int* comparacion_prefijo(const char* a, const char* b){
	unsigned int numIzq = atoi(a);
	unsigned int numDer = atoi(b);
	return b-a;
}

void codificador_codificarBinarioPrefijo(buffer_t* buffer, unsigned int num, unsigned int b){

	arbol_huff_t* arbol = arbol_huff_crear((cmp_func_t)comparacion_prefijo, b);

	if(!arbol)
		return;
	
	lista_t* lista_bits = arbol_huff_obtener_lista_bits(arbol,num);
	
	while(!lista_esta_vacia(lista_bits)){
		Byte_t* numero = lista_borrar_primero(lista_bits);
		buffer_escribir_bit(buffer, *numero);
		if (*numero == 0)
			printf("imprime: 0 \n" );
		if (*numero == 1)
			printf("imprime: 1 \n" );
		free(numero);
	}
	
	lista_destruir(lista_bits, free);
	
	arbol_huff_destruir(arbol);

}

void codificador_codificarGolomb(buffer_t* buffer, unsigned int num, unsigned int b){
	if ((b == 0) || (num == 0))
		return;
	
	unsigned int q = (num - 1) / b;
	unsigned int r = num - (q * b) - 1;
	
	codificador_codificarUnario(buffer, q + 1);
	codificador_codificarBinarioPrefijo(buffer, r, b);
}


void codificador_destruir(codificador_t* cod){
	free(cod);
}

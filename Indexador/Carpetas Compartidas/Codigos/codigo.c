#include "codigo.h"
#include <stddef.h>
#include "matematicaEspecial.h"
#include <stdio.h>
#include "../Manejo de Archivos/buffer.h"

#define COMP_GAMMA 0
#define COMP_DELTA 1
#define TAM_BYTE 8

struct codigo{
	Byte_t* code;
	size_t cant_bits;
};

codigo_t* codigo_crearUnario(unsigned int);
codigo_t* codigo_crearBinario(unsigned int, size_t);
codigo_t* codigo_crearGamma(unsigned int);
codigo_t* codigo_crearDelta(unsigned int);
size_t cantidadDeBytes(size_t cant_bits);

codigo_t* codigo_crear(unsigned int num, unsigned int tipo){
	switch(tipo){
		case CODIGO_UNARIO: return codigo_crearUnario(num);
		case CODIGO_BINARIO: return codigo_crearBinario(num,0);
		case CODIGO_GAMMA: return codigo_crearGamma(num);
		case CODIGO_DELTA: return codigo_crearDelta(num);
		default: return NULL;
	}
}


void codigo_destruir(codigo_t* code){
	if (!code) return;
	free(code->code);
	free(code);
}


size_t cantidadDeBytes(size_t cant_bits){
	//Devuelve la cantidad de bytes que ocupa una cierta cantidad de bits
	
	size_t sum = 0;
	if (cant_bits % BITS_X_BYTE != 0) sum = 1;
	//si el resto es 0, la division de mas abajo dara justo
	//sino, debo sumarle uno para que entren todos los bits


	size_t cant = cant_bits / BITS_X_BYTE + sum;
	return cant;
}


codigo_t* codigo_crearUnario(unsigned int num){
	//crear un codigo unario a partir de un numero
	if (num == 0) return NULL;

	codigo_t* unario = malloc (sizeof(codigo_t));
	if (!unario) return NULL;

	unario->cant_bits = num; //va a tener (num-1) 1's, y un 0

	//Me fijo cuantos bytes voy a necesitar para guardar todos los bits
	size_t cantBytes = cantidadDeBytes(unario->cant_bits);
	
	//Reservo la memoria suficiente
	unario->code = malloc (sizeof(Byte_t) * cantBytes);
	if (!unario->code){
		free (unario);
		return NULL;
	}
	
	//Inicializo todos los bytes en 0
	for (size_t i = 0; i < cantBytes; i++)
		unario->code[i] = 0;

	//Voy a poner los (n-1) 1's cant_
	size_t byteActual = 0;
	unsigned int pot = 7; //Pos maxima en el byte
	for (size_t j = 0; j < (num-1); j++){
		unario->code[byteActual] += Byte_dosElevadoALa(pot);
		if (pot == 0){
			pot = 7;
			byteActual++;
		}else
			pot--;
	}

	return unario;
}

codigo_t* codigo_crearBinario(unsigned int num, size_t longitud ){
	if (longitud == 0)
		return NULL;
	
	unsigned max = maxPot2(num);
	int diferencia = (longitud - 1) - max;
	if (diferencia < 0)
		return NULL;

	codigo_t* binario = malloc (sizeof(codigo_t));
	if (!binario) return NULL;
	
	size_t cantBytes = cantidadDeBytes(longitud);
	binario->code = malloc(sizeof(Byte_t) * cantBytes);
	if(!binario->code){
		free(binario);
		return NULL;
	}

	for (size_t i = 0; i < cantBytes; i++)
		binario->code[i] = 0;
	binario->cant_bits = longitud;

	/* FALTA PONER LO QUE HAGA QUE SE PONGO EL BINARIO*/

	return binario;
}

codigo_t* codigo_crearCompresor(unsigned int num, unsigned int tipo_codigo){
	codigo_t* cod_compresor = malloc (sizeof(codigo_t));
	if (!cod_compresor) return NULL;
		
	//Obtengo los numeros que tengo que escribir
	unsigned int piso_log = maxPot2(num);
	unsigned int num_primera_parte = 1 + piso_log;
	unsigned int num_binario = num - dosElevadoALa(piso_log);
	size_t cant_bits_binario = piso_log;
		
	//Creo los codigos de cada tipo
	codigo_t* cod_primera_parte;
	if (tipo_codigo == COMP_GAMMA){
		cod_primera_parte = codigo_crearUnario(num_primera_parte);
	}else{
		cod_primera_parte = codigo_crearGamma(num_primera_parte);
	}
	
	codigo_t* cod_binario = codigo_crearBinario(num_binario, cant_bits_binario);
		
	if(!cod_primera_parte || !cod_binario){
		free(cod_compresor);
		codigo_destruir(cod_primera_parte);
		codigo_destruir(cod_binario);
		return NULL;
	}
			
	size_t cantidad_bits = (cod_primera_parte->cant_bits + cod_binario->cant_bits);
	cod_compresor->cant_bits = cantidad_bits;
		
	size_t cantBytes = cantidadDeBytes(cantidad_bits);
	cod_compresor->code = malloc(sizeof(Byte_t) * cantBytes);
	
	if(!cod_compresor->code){
		codigo_destruir(cod_primera_parte);
		codigo_destruir(cod_binario);
		free(cod_compresor);
		return NULL;
	}
		
	//Paso cada codigo al cod_compresor
	buffer_t* buffer_bits = buffer_crear(NULL);
	unsigned int cant_bits_restantes = cod_primera_parte->cant_bits;
	unsigned int num_byte = cantidadDeBytes(cant_bits_restantes);
	unsigned int byte_actual = cantBytes; 
	while(cant_bits_restantes >= 8){
		cod_compresor->code[byte_actual] = cod_primera_parte->code[num_byte];
		cant_bits_restantes -= 8;
		num_byte --;
		byte_actual --;
	}
	
	
	//Destruyo los codigos que cree temporalmente
	codigo_destruir(cod_primera_parte);
	codigo_destruir(cod_binario);

	return cod_compresor;
}

codigo_t* codigo_crearGamma(unsigned int num){
	return codigo_crearCompresor(num, COMP_GAMMA);
}

codigo_t* codigo_crearDelta(unsigned int num){
	return codigo_crearCompresor(num, COMP_DELTA);
}


void codigo_imprimir(codigo_t* c){
	if (!c) return;
	size_t cant_bytes = cantidadDeBytes(c->cant_bits);
	printf("Codigo: ");
	for (size_t i = 0; i < cant_bytes; i++)
		printf("%d + ", c->code[i]);
	printf("\n");
}

#include "codigo.h"
#include <stddef.h>
#include <stdlib.h>
#include "matematicaEspecial.h"
#include <stdio.h>

struct codigo{
	Byte_t* code;
	size_t bits;
};

codigo_t* codigo_crearUnario(unsigned int);
codigo_t* codigo_crearBinario(unsigned int, size_t);
codigo_t* codigo_crearGamma(unsigned int a){return NULL;}
codigo_t* codigo_crearDelta(unsigned int a){return NULL;}
size_t cantidadDeBytes(size_t bits);

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


size_t cantidadDeBytes(size_t bits){
	//Devuelve la cantidad de bytes que ocupa una cierta cantidad de bits
	size_t sum = 0;
	if (bits % BITS_X_BYTE != 0) sum = 1;
	//si el resto es 0, la division de mas abajo dara justo
	//sino, debo sumarle uno para que entren todos los bits


	size_t cant = bits / BITS_X_BYTE + sum;
	return cant;
}


codigo_t* codigo_crearUnario(unsigned int num){
	//crear un codigo unario a partir de un numero
	if (num == 0) return NULL;

	codigo_t* unario = malloc (sizeof(codigo_t));
	if (!unario) return NULL;

	unario->bits = num; //va a tener (num-1) 1's, y un 0

	//Me fijo cuantos bytes voy a necesitar para guardar todos los bits
	size_t cantBytes = cantidadDeBytes(unario->bits);
	//Reservo la memoria suficiente
	unario->code = malloc (sizeof(Byte_t) * cantBytes);
	if (!unario->code){free (unario); return NULL;}
	//Inicializo todos los bytes en 0
	for (size_t i = 0; i < cantBytes; i++)
		unario->code[i] = 0;

	//Voy a poner los (n-1) 1's
	size_t byteActual = 0;
	unsigned int pot = 7;
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
	if (longitud == 0){
		return NULL;
	}
	unsigned max = maxPot2(num);
	int diferencia = (longitud - 1) - max;
	if (diferencia < 0) return NULL;

	codigo_t* binario = malloc (sizeof(codigo_t));
	if (!binario) return NULL;
	size_t cantBytes = cantidadDeBytes(longitud);
	binario->code = malloc(sizeof(Byte_t) * cantBytes);
	if(!binario->code){free(binario); return NULL;}

	for (size_t i = 0; i < cantBytes; i++)
		binario->code[i] = 0;
	binario->bits = longitud;

	/* FALTA PONER LO QUE HAGA QUE SE PONGO EL BINARIO*/

	return binario;
}


void codigo_imprimir(codigo_t* c){
	if (!c) return;
	size_t cant_bytes = cantidadDeBytes(c->bits);
	printf("Codigo: ");
	for (size_t i = 0; i < cant_bytes; i++)
		printf("%d + ", c->code[i]);
	printf("\n");
}

#include "matematicaEspecial.h"

#define POTENCIA_MAX_BYTE 7
const Byte_t POTENCIAS_DE_2_BYTE[] = {1,2,4,8,16,32,64,128};

#define POTENCIA_MAX 16
const unsigned int POTENCIAS_DE_2[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768, 65536};

Byte_t Byte_dosElevadoALa(unsigned int pot){
	//Por ahora lo dejamos semi-hardcodeado
	//Luego vemos si es mejor hacer shifts y esas cosas

	if (pot > POTENCIA_MAX_BYTE) return 0;
	return POTENCIAS_DE_2_BYTE[pot];
}

unsigned int dosElevadoALa(unsigned int pot){
	if (pot > POTENCIA_MAX) return POTENCIAS_DE_2[POTENCIA_MAX]*dosElevadoALa(pot-POTENCIA_MAX);
	return POTENCIAS_DE_2[pot];
}

unsigned int maxPot2(unsigned int num){
	unsigned int pot = 0;
	int resul;
	while ( (resul =(num - dosElevadoALa(pot))) >= 0){
		pot++;
	}

	return pot-1;
}

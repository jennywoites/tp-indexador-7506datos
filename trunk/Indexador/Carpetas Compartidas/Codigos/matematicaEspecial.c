#include "matematicaEspecial.h"
#include <math.h>

#define POTENCIA_MAX_BYTE 7
const Byte_t POTENCIAS_DE_2_BYTE[] = {1,2,4,8,16,32,64,128};

#define POTENCIA_MAX 16
const unsigned int POTENCIAS_DE_2[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768, 65536};

const float LOG_2[] = {-4.32, -3.32, -2.736, -2.322, -2, -1.737, -1.515, -1.322, -1.152, -1, -0.86, -0.736, -0.621, -0.514, -0.415,-0.322,-0.234,-0.152,-0.0074,0, 0.07, 0.137, 0.202, 0.263, 0.322, 0.379, 0.433, 0.485, 0.536, 0.585, 0.6322, 0.678, 0.722, 0.7655, 0.807, 0.848, 0.8875, 0.92599, 0.9634, 1};
#define PASO 0.05

Byte_t Byte_dosElevadoALa(unsigned int pot){
	//Por ahora lo dejamos semi-hardcodeado
	//Luego vemos si es mejor hacer shifts y esas cosas

	if (pot > POTENCIA_MAX_BYTE) return 0;
	return POTENCIAS_DE_2_BYTE[pot];
}

unsigned int dosElevadoALa(unsigned int pot){
	if (pot > POTENCIA_MAX)
		return POTENCIAS_DE_2[POTENCIA_MAX]*dosElevadoALa(pot-POTENCIA_MAX);
	
	return POTENCIAS_DE_2[pot];
}

unsigned int maxPot2(unsigned int num){
	//Devuelve la potencia del numero en base dos mas cercano (por debajo)
	// piso(log2(num))
	
	unsigned int pot = 0;
	int resul;
	while ( (resul =(num - dosElevadoALa(pot))) >= 0){
		pot++;
	}

	return pot-1;
}


size_t calcular_B_optimo(float p){
	//FIXME:

	float b_float = - log(2-p) / log(1-p);

//	if (p>1){
//		printf("%f \n", log(1-p));
//		getchar();
//	}

	size_t b = b_float;
	if (b < b_float) b++;

	return b;
}

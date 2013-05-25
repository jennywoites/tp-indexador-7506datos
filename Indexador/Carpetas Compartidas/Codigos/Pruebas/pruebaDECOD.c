#include "../../Manejo de Archivos/debuffer.h"
#include "../decodificador.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void ok(unsigned int num1, unsigned int num2){
	if (num1 != num2)
		printf("ERROR!!!!!!!!!!!!!!!!!!!!!!, %d\n", num2);
}
int main (){
	//para pruebas, hay que pasarse al fprintf en el buffer.c
	FILE* file = fopen("buffASCII.jem","r");
	debuffer_t* debuff = debuffer_crear(file);

	decodificador_t* decod = decodificador_crear(debuff);
	
	ok(decodificador_decodificarUnario(decod), 10);
	ok(decodificador_decodificarUnario(decod), 6);
	ok(decodificador_decodificarGamma(decod), 9);
	ok(decodificador_decodificarGamma(decod), 8);
	ok(decodificador_decodificarUnario(decod), 2);
	ok(decodificador_decodificarGamma(decod), 17);
	ok(decodificador_decodificarGamma(decod), 15);
	ok(decodificador_decodificarDelta(decod), 9);
	ok(decodificador_decodificarUnario(decod), 6);
	ok(decodificador_decodificarDelta(decod), 6);
	ok(decodificador_decodificarGamma(decod),5);
	
	ok(decodificador_decodificarDelta(decod), 117);
	ok(decodificador_decodificarUnario(decod), 5);
	
	ok(decodificador_decodificarGamma(decod), 232);
	ok(decodificador_decodificarUnario(decod), 1);
	
	ok(decodificador_decodificarDelta(decod), 4037);
	ok(decodificador_decodificarUnario(decod), 6);

	debuffer_destruir(debuff);
	decodificador_destruir(decod);
	fclose(file);
	return 0;
}

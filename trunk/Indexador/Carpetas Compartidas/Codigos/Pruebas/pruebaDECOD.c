#include "../../Manejo de Archivos/debuffer.h"
#include "../decodificador.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

void ok(unsigned int num1, unsigned int num2){
	if (num1 != num2)
		printf("ERROR!!!!!!!!!!!!!!!!!!!!!!, %d y tiro un %d\n", num2, num1);
	else
		printf("OK\n");
}
int mainDECOD (){
	FILE* file = fopen("buffASCII.jem","r");
	struct stat st;
	stat("buffASCII.jem", &st);
	size_t cant_bytes = st.st_size;
	debuffer_t* decod = debuffer_crear(file, cant_bytes);
		
	
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
	
	ok(decodificador_decodificarDelta(decod), 40370);
	ok(decodificador_decodificarGamma(decod), 240370);
	ok(decodificador_decodificarUnario(decod), 4);
	
	ok(decodificador_decodificarUnario(decod), NO_NUMERO);

	debuffer_destruir(decod);
	fclose(file);
	return 0;
}

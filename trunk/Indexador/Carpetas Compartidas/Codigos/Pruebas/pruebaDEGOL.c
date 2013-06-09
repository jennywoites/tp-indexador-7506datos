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
int main (){
	//para pruebas, hay que pasarse al fprintf en el buffer.c
	FILE* file = fopen("gol.jem","r");
	struct stat st;
	stat("gol.jem", &st);
	size_t cant_bytes = st.st_size;
	debuffer_t* decod = debuffer_crear(file, cant_bytes);

	//codificador_t* cod = codificador_crear(buff);
	
	ok(decodificador_decodificarGolomb(decod, 20), 1);
	ok(decodificador_decodificarGolomb(decod, 20), 2);
	ok(decodificador_decodificarGolomb(decod, 20), 3);
	ok(decodificador_decodificarGolomb(decod, 20), 4);
	ok(decodificador_decodificarGolomb(decod, 20), 5);
	ok(decodificador_decodificarGolomb(decod, 20), 6);
	ok(decodificador_decodificarGolomb(decod, 20), 7);
	ok(decodificador_decodificarGolomb(decod, 20), 8);
	ok(decodificador_decodificarGolomb(decod, 20), 9);
	ok(decodificador_decodificarGolomb(decod, 20), 10);
	ok(decodificador_decodificarGolomb(decod, 20), 11);
	ok(decodificador_decodificarGolomb(decod, 20), 12);
	ok(decodificador_decodificarGolomb(decod, 20), 13);
	ok(decodificador_decodificarGolomb(decod, 20), 14);
	ok(decodificador_decodificarGolomb(decod, 20), 15);
	ok(decodificador_decodificarGolomb(decod, 20), 160);
	ok(decodificador_decodificarGolomb(decod, 20), 100);
	ok(decodificador_decodificarGolomb(decod, 20), 18);
	ok(decodificador_decodificarGolomb(decod, 20), 19);

	debuffer_destruir(decod);

	fclose(file);
	return 0;
}

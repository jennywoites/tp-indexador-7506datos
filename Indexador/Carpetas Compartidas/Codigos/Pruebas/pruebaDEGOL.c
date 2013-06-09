#include "../../Manejo de Archivos/debuffer.h"
#include "../decodificador.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>


void okGOL(unsigned int num1, unsigned int num2){
	if (num1 != num2)
		printf("ERROR!!!!!!!!!!!!!!!!!!!!!!, %d y tiro un %d\n", num2, num1);
	else
		printf("OK\n");
}
int mainDEGOL (){
	//para pruebas, hay que pasarse al fprintf en el buffer.c
	FILE* file = fopen("gol.jem","r");
	struct stat st;
	stat("gol.jem", &st);
	size_t cant_bytes = st.st_size;
	debuffer_t* decod = debuffer_crear(file, cant_bytes);

	//codificador_t* cod = codificador_crear(buff);
	
	okGOL(decodificador_decodificarGolomb(decod, 20), 1);
	okGOL(decodificador_decodificarGolomb(decod, 20), 2);
	okGOL(decodificador_decodificarGolomb(decod, 20), 3);
	okGOL(decodificador_decodificarGolomb(decod, 20), 4);
	okGOL(decodificador_decodificarGolomb(decod, 20), 5);
	okGOL(decodificador_decodificarGolomb(decod, 20), 6);
	okGOL(decodificador_decodificarGolomb(decod, 20), 7);
	okGOL(decodificador_decodificarGolomb(decod, 20), 8);
	okGOL(decodificador_decodificarGolomb(decod, 20), 9);
	okGOL(decodificador_decodificarGolomb(decod, 20), 10);
	okGOL(decodificador_decodificarGolomb(decod, 20), 11);
	okGOL(decodificador_decodificarGolomb(decod, 20), 12);
	okGOL(decodificador_decodificarGolomb(decod, 20), 13);
	okGOL(decodificador_decodificarGolomb(decod, 20), 14);
	okGOL(decodificador_decodificarGolomb(decod, 20), 15);
	okGOL(decodificador_decodificarGolomb(decod, 20), 160);
	okGOL(decodificador_decodificarGolomb(decod, 20), 100);
	okGOL(decodificador_decodificarGolomb(decod, 20), 18);
	okGOL(decodificador_decodificarGolomb(decod, 20), 19);

	debuffer_destruir(decod);

	fclose(file);
	return 0;
}

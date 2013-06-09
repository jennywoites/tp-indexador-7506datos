#include "../../Manejo de Archivos/buffer.h"
#include "../codificador.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int mainGOL (){
	//para pruebas, hay que pasarse al fprintf en el buffer.c
	FILE* file = fopen("gol.jem","w");
	buffer_t* cod = buffer_crear(file);

	//codificador_t* cod = codificador_crear(buff);
	
	printf("el 0\n");
	codificador_codificarBinarioPrefijo(cod, 0, 20);
	printf("el 1\n");
	codificador_codificarBinarioPrefijo(cod, 1, 20);
	printf("el 2\n");
	codificador_codificarBinarioPrefijo(cod, 2, 20);
	printf("el 3\n");
	codificador_codificarBinarioPrefijo(cod, 3, 20);
	printf("el 4\n");
	codificador_codificarBinarioPrefijo(cod, 4, 20);
	printf("el 5\n");
	codificador_codificarBinarioPrefijo(cod, 5, 20);
	printf("el 6\n");
	codificador_codificarBinarioPrefijo(cod, 6, 20);
	printf("el 7\n");
	codificador_codificarBinarioPrefijo(cod, 7, 20);
	printf("el 8\n");
	codificador_codificarBinarioPrefijo(cod, 8, 20);
	printf("el 9\n");
	codificador_codificarBinarioPrefijo(cod, 9, 20);
	printf("el 10\n");
	codificador_codificarBinarioPrefijo(cod, 10, 20);
	printf("el 11\n");
	codificador_codificarBinarioPrefijo(cod, 11, 20);
	printf("el 12\n");
	codificador_codificarBinarioPrefijo(cod, 12, 20);
	printf("el 13\n");
	codificador_codificarBinarioPrefijo(cod, 13, 20);
	printf("el 14\n");
	codificador_codificarBinarioPrefijo(cod, 14, 20);
	printf("el 15\n");
	codificador_codificarBinarioPrefijo(cod, 15, 20);
	printf("el 16\n");
	codificador_codificarBinarioPrefijo(cod, 16, 20);
	printf("el 17\n");
	codificador_codificarBinarioPrefijo(cod, 17, 20);
	printf("el 18\n");
	codificador_codificarBinarioPrefijo(cod, 18, 20);
	printf("el 19\n");
	codificador_codificarBinarioPrefijo(cod, 19, 20);
	
	buffer_rellenar(cod);

	buffer_destruir(cod);
	//codificador_destruir(cod);
	fclose(file);
	return 0;
}

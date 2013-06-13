#include "../../Manejo de Archivos/buffer.h"
#include "../codificador.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int mainPruebasGol (){
	//para pruebas, hay que pasarse al fprintf en el buffer.c
	FILE* file = fopen("gol.jem","w");
	buffer_t* cod = buffer_crear(file);

	//codificador_t* cod = codificador_crear(buff);
	
	arbol_huff_t * a = arbol_huff_crear(394764);
	for (unsigned int i = 1; i < 4000; i++){
		codificador_codificarGolomb(cod, i, 394764, a);
		i += 7;
	}
	

	arbol_huff_destruir(a);
	/*printf("el 1\n");
	codificador_codificarGolomb(cod, 1, 20);
	printf("el 2\n");
	codificador_codificarGolomb(cod, 2, 20);
	printf("el 3\n");
	codificador_codificarGolomb(cod, 3, 20);
	printf("el 4\n");
	codificador_codificarGolomb(cod, 4, 20);
	printf("el 5\n");
	codificador_codificarGolomb(cod, 5, 20);
	printf("el 6\n");
	codificador_codificarGolomb(cod, 6, 20);
	printf("el 7\n");
	codificador_codificarGolomb(cod, 7, 20);
	printf("el 8\n");
	codificador_codificarGolomb(cod, 8, 20);
	printf("el 9\n");
	codificador_codificarGolomb(cod, 9, 20);
	printf("el 10\n");
	codificador_codificarGolomb(cod, 10, 20);
	printf("el 11\n");
	codificador_codificarGolomb(cod, 11, 20);
	printf("el 12\n");
	codificador_codificarGolomb(cod, 12, 20);
	printf("el 13\n");
	codificador_codificarGolomb(cod, 13, 20);
	printf("el 14\n");
	codificador_codificarGolomb(cod, 14, 20);
	printf("el 15\n");
	codificador_codificarGolomb(cod, 15, 20);
	printf("el 16\n");
	codificador_codificarGolomb(cod, 160, 20);
	printf("el 17\n");
	codificador_codificarGolomb(cod, 100, 20);
	printf("el 18\n");
	codificador_codificarGolomb(cod, 18, 20);
	printf("el 19\n");
	codificador_codificarGolomb(cod, 19, 20);
*/
	
	buffer_rellenar(cod);

	buffer_destruir(cod);
	//codificador_destruir(cod);
	fclose(file);
	return 0;
}

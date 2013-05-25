#include "../../Manejo de Archivos/buffer.h"
#include "../codificador.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main (){
	//para pruebas, hay que pasarse al fprintf en el buffer.c
	FILE* file = fopen("buffASCII.jem","w");
	buffer_t* buff = buffer_crear(file);

	codificador_t* cod = codificador_crear(buff);
	
	codificador_codificarUnario(cod, 10);
	codificador_codificarUnario(cod, 6);
	codificador_codificarGamma(cod, 9);
	codificador_codificarGamma(cod, 8);
	codificador_codificarUnario(cod, 2);
	codificador_codificarGamma(cod, 17);
	codificador_codificarGamma(cod, 15);
	codificador_codificarDelta(cod, 9);
	codificador_codificarUnario(cod, 6);
	codificador_codificarDelta(cod, 6);
	codificador_codificarGamma(cod,5);
	
	codificador_codificarDelta(cod, 117);
	codificador_codificarUnario(cod, 5);
	
	codificador_codificarGamma(cod, 232);
	codificador_codificarUnario(cod, 1);

	codificador_codificarDelta(cod, 4037);
	codificador_codificarUnario(cod, 6);

	buffer_destruir(buff);
	codificador_destruir(cod);
	fclose(file);
	return 0;
}

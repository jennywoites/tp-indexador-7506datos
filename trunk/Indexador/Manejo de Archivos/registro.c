#include "registro.h"
#include <stdlib.h>

struct registro{
	//Por ahora tiene un simple char:
	char c;
};

int comparacionRegistros(const void* a, const void* b){
	dato_t* r1 = (dato_t*) a;
	dato_t* r2 = (dato_t*) b;
	return r2->registro->c - r1->registro->c;
}


registro_t* registro_leer(FILE* entrada){
	registro_t* reg = malloc (sizeof(registro_t));
	if (!reg) return NULL;

	reg->c = fgetc(entrada);
	if (reg->c == EOF || feof(entrada)){
		free(reg);
		return NULL;
	}

	return reg;
}

void registro_escribir(FILE* salida, registro_t* reg){
	if (!reg) return;
	fprintf(salida, "%c",reg->c);
}

void registro_destruir(registro_t* reg){
	free(reg);
}

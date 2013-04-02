#include "funcionesGeneralesArchivos.h"

const char* LECTURA = "r";
const char* ESCRITURA = "w";
bool lectura_anticipada(FILE* arch, char* c){
	if (feof(arch)) return false;
	(*c) = fgetc (arch);
	if ((*c)==EOF) return false;
	return true;
}


const char* lectura_archivos(){
	return LECTURA;
}
const char* escritura_archivos(){
	return ESCRITURA;
}

#include "funcionesGeneralesArchivos.h"

bool lectura_anticipada(FILE* arch, char* c){
	if (feof(arch)) return false;
	(*c) = fgetc (arch);
	return true;
}


const char* lectura_archivos(){
	return "r";
}
const char* escritura_archivos(){
	return "w";
}

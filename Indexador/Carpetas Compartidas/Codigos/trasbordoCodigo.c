#include "trasbordoCodigo.h"
#include "../TDAs/hash.h"
#include "../TDAs/arbolHuff.h"

hash_t* arboles = NULL;

void destructorArbol(void* a){
	arbol_huff_t* arbol = (arbol_huff_t*)a;
	arbol_huff_destruir(arbol);
}


arbol_huff_t* obtenerArbolito(size_t b){
	if (!arboles) arboles = hash_crear(destructorArbol);
	char* clave = malloc (sizeof(char)*50);
	sprintf(clave,"%lu", b);
	arbol_huff_t* arbolito;
	if (hash_pertenece(arboles, clave)){
		arbolito = hash_obtener(arboles, clave);
	}else{
		arbolito = arbol_huff_crear(b);
		hash_guardar(arboles, clave, arbolito);
	}
	free(clave);
	return arbolito;
}

void comprimir_FrecuenciaDocumentos(buffer_t* cod, unsigned int num){
	codificador_codificarDelta(cod,num);
}

void comprimir_FrecuenciaPosiciones(buffer_t* cod, unsigned int num){
	codificador_codificarDelta(cod,num);
}

void comprimir_IndiceDistanciaDocumentos(buffer_t* cod, unsigned int num, size_t b){
	//codificador_codificarGamma(cod,num);
	arbol_huff_t* arbolito = obtenerArbolito(b);
	codificador_codificarGolomb(cod, num, b, arbolito);
}

void comprimir_IndiceDistanciaPosiciones(buffer_t* cod, unsigned int num, size_t b){
	//codificador_codificarDelta(cod, num);
	arbol_huff_t* arbolito = obtenerArbolito(b);
	codificador_codificarGolomb(cod, num , b, arbolito);
}

void comprimir_LexicoRepetidos(buffer_t* cod, unsigned int num){
	codificador_codificarGamma(cod,num);
}

void comprimir_LexicoDiferentes(buffer_t* cod, unsigned int num){
	codificador_codificarGamma(cod,num);
}

void comprimir_LexicoOffset(buffer_t* cod, unsigned int num){
	codificador_codificarDelta(cod,num);
}

unsigned int descomprimir_FrecuenciaDocumentos(debuffer_t* cod){
	return decodificador_decodificarDelta(cod);
}

unsigned int descomprimir_FrecuenciaPosiciones(debuffer_t* cod){
	return decodificador_decodificarDelta(cod);
}

unsigned int descomprimir_IndiceDistanciaDocumentos(debuffer_t* cod, size_t b){
	//return decodificador_decodificarGamma(cod);
	arbol_huff_t* arbolito = obtenerArbolito(b);
	return decodificador_decodificarGolomb(cod, b, arbolito);
}

unsigned int descomprimir_IndiceDistanciaPosiciones(debuffer_t* cod, size_t b){
	//return decodificador_decodificarDelta(cod);
	arbol_huff_t* arbolito = obtenerArbolito(b);
	return decodificador_decodificarGolomb(cod, b, arbolito);
}

unsigned int descomprimir_LexicoRepetidos(debuffer_t* cod){
	return decodificador_decodificarGamma(cod);
}

unsigned int descomprimir_LexicoDiferentes(debuffer_t* cod){
	return decodificador_decodificarGamma(cod);
}

unsigned int descomprimir_LexicoOffset(debuffer_t* cod){
	return decodificador_decodificarDelta(cod);
}

void comprimir_BGolomb(buffer_t* cod, unsigned int num){
	codificador_codificarGamma(cod, num);
}

unsigned int descomprimir_BGolomb(debuffer_t* cod){
	return decodificador_decodificarGamma(cod);
}

void destruirTrasbordo(){
	hash_destruir(arboles);
	arboles = NULL;
}

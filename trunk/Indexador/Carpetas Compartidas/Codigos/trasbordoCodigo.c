#include "trasbordoCodigo.h"
#include "../TDAs/hash.h"
#include "../TDAs/arbolHuff.h"

#define CANT_ALMACENADOS 10000000
hash_t* arboles = NULL;

void destructorArbol(void* a){
	arbol_huff_t* arbol = (arbol_huff_t*)a;
	arbol_huff_destruir(arbol);
}

arbol_huff_t* obtenerArbolito(size_t b, int *borrado){
	//if (1)
	//	return arbol_huff_crear(b);
	if (!arboles) arboles = hash_crear(destructorArbol);
	char* clave = malloc (sizeof(char)*50);
	sprintf(clave,"%lu", b);
	arbol_huff_t* arbolito;
	*borrado = 0;
	if (hash_pertenece(arboles, clave)){
		arbolito = hash_obtener(arboles, clave);
	}else{
		arbolito = arbol_huff_crear(b);
		if (b < 300) *borrado = 1;
		else if (b > 20000) *borrado = 1;
		else if (hash_cantidad(arboles) <= CANT_ALMACENADOS )
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
	int borrado;
//	if(b>10000){
//		printf("%lu \n", -b);
//		getchar();
//	}
	arbol_huff_t* arbolito = obtenerArbolito(b, &borrado);
	codificador_codificarGolomb(cod, num, b, arbolito);
	if (borrado) arbol_huff_destruir(arbolito);
}

void comprimir_IndiceDistanciaPosiciones(buffer_t* cod, unsigned int num, size_t b){
	//codificador_codificarDelta(cod, num);
	int borrado;
	arbol_huff_t* arbolito = obtenerArbolito(b, &borrado);
	codificador_codificarGolomb(cod, num, b, arbolito);
	if (borrado) arbol_huff_destruir(arbolito);
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
	int borrado;
	arbol_huff_t* arbolito = obtenerArbolito(b, &borrado);
	unsigned int devolver = decodificador_decodificarGolomb(cod, b, arbolito);
	if (borrado) arbol_huff_destruir(arbolito);
	return devolver;
}

unsigned int descomprimir_IndiceDistanciaPosiciones(debuffer_t* cod, size_t b){
	//return decodificador_decodificarDelta(cod);
	int borrado;
	arbol_huff_t* arbolito = obtenerArbolito(b, &borrado);
	unsigned int devolver = decodificador_decodificarGolomb(cod, b, arbolito);
	if (borrado) arbol_huff_destruir(arbolito);
	return devolver;
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

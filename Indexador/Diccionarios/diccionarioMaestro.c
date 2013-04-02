#include "diccionarioMaestro.h"
#include "../TDAs/hash.h"
#include <stddef.h>
#include <stdlib.h>

hash_t* DICCIONARIO = NULL;

void cargar_datos(hash_t* hash){
	//aca iria la carga, pero por ahora no hago nada
}

void diccionario_cargar(/*const char* ruta*/){
	DICCIONARIO = hash_crear(NULL); //los datos a guardar seran NULL.
	cargar_datos(DICCIONARIO);
}

void diccionario_liberar(){
	hash_destruir(DICCIONARIO);
}

bool diccionario_palabraEsClave(char* palabra){
	return !hash_pertenece(DICCIONARIO, palabra);
}

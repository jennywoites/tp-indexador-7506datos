#include "diccionarioMaestro.h"
#include "../TDAs/hash.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Manejo de Archivos/funcionesGeneralesArchivos.h"

hash_t* DICCIONARIO = NULL;
const char* RUTA_DEFAULT = "Diccionarios/dicc.txt";

void cargar_datos(hash_t* hash, const char* ruta){
	FILE* arch = fopen(ruta, lectura_archivos());
	if (!arch) return;

	unsigned int cant;
	char** lineas = obtenerLineas(arch, &cant);
	//El archivo tendra en minusculas los articulos, uno por linea
	for (unsigned int i = 0; i < cant; i++){
		hash_guardar(hash, lineas[i],NULL );
		//El hash se guarda una copia de la clave, entonces:
		free(lineas[i]);
	}
	free(lineas);
	fclose(arch);
}

void diccionario_cargar(const char* ruta){
	DICCIONARIO = hash_crear(NULL); //los datos a guardar seran NULL.
	if (!ruta)
		cargar_datos (DICCIONARIO, RUTA_DEFAULT);
	else
		cargar_datos(DICCIONARIO, ruta);
}

void diccionario_liberar(){
	hash_destruir(DICCIONARIO);
}

bool diccionario_palabraEsClave(char* palabra){
	return !hash_pertenece(DICCIONARIO, palabra);
}

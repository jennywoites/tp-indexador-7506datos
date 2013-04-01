#include "merger.h"
#include "funcionesGeneralesArchivos.h"
#include "../TDAs/abb.h"
#include <stddef.h>
#define CANT_ARCHIVOS_SEGUIDOS 10
#define CANT_REGISTROS_POR_ARCHIVO 10

const char* SALIDA = "salida.txt"; 	//salida que se usara para el merge


//ACLARACION: se hace un merge multi-etapa

FILE** abrir_archivos(char**, int, unsigned int);
void cerrar_archivos(FILE**, int);
void agregar_elementos(abb_t*, FILE**, int);
char* procesar_archivos(abb_t*, FILE**, int, unsigned int, FILE*);


char* merger(char** rutas, unsigned int i, int cant, FILE* salida){

	FILE** archs = abrir_archivos(rutas, cant,i);
	abb_t* arbol = abb_crear(NULL, NULL); 	//deberiamos pasarle funcion de comparacion, y cambiar que la clave tenga que
											//si o si un char*
	agregar_elementos(arbol, archs, cant);

	char* ruta_aux = procesar_archivos(arbol, archs, cant, i, salida);

	cerrar_archivos(archs, cant);
	return ruta_aux;
}


int merger_MergearArchivos(char** rutas, int cant){
	if (cant <= 0) return MERGER_ERROR;

	FILE* archSalida = fopen (SALIDA, escritura_archivos());
	if (!archSalida) return MERGER_ERROR;
	int c = cant/CANT_ARCHIVOS_SEGUIDOS;
	//Primera etapa del merge. Subdivido por la constante CANT_ARCHIVOS_SEGUIDOS a tratar,
	//abro cada archivo, creo el arbol con un elemento de cada archivo, y voy avanzando en cada uno.
	char* rutas_aux[c];
	for (unsigned int i = 0; i < c; i++){
		rutas_aux[i] = merger(rutas, i, CANT_ARCHIVOS_SEGUIDOS, NULL);
	}

	merger(rutas_aux, 0, c, archSalida);

	return MERGER_OK;
}


FILE** abrir_archivos(char** rutas, int cantidad, unsigned int i){
	FILE** archivos = malloc (sizeof(FILE*)* cantidad);
	for (unsigned int j = i*cantidad; j < ((i+1)*cantidad);j++){
		archivos[j] = fopen(rutas[i*cantidad+j], lectura_archivos());
	}
	return archivos;
}

void cerrar_archivos(FILE** archivos, int cantidad){
	for (unsigned int j = 0; j < cantidad; j++)
		fclose(archivos[j]);
	free(archivos);
}

void agregar_elementos(abb_t* arbol, FILE** archivos, int cant){
	for (unsigned int j = 0; j < cant; j++){
		for (unsigned int k = 0; k < CANT_REGISTROS_POR_ARCHIVO;k++){
			//registro_t* registro = leer_registro(archivos[i]);
			char* clave = NULL;	// = registro_clave(registro);
			void* dato = NULL;	// = registro_dato(registro) + j (?), necesito guardarlo aca para saber luego de cual sacar.
			abb_guardar(arbol,clave , dato);
		}
	}
}

char* procesar_archivos(abb_t* arbol, FILE** archivos, int cant, unsigned int i, FILE* useifnotNULL){
	//esto es lo que no tengo idea de como hacer todavia u.u
	return NULL;
}

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "merger.h"
#include "funcionesGeneralesArchivos.h"
#include "../Carpetas Compartidas/TDAs/heap.h"
#include "registro.h"
#define CANT_ARCHIVOS_SEGUIDOS 10
#define CANT_REGISTROS_POR_ARCHIVO 10

const char* SALIDA = "salida.txt"; 			//salida que se usara para el merge
const char* SALIDA_TEMPORAL = "temp.txt";	//salida para archivos temporales

/*Definicion del tipo de datos a guardar dentro del heap para hacer de auxiliar en el merge*/


//ACLARACION: se hace un merge multi-etapa

FILE** abrir_archivos(char**, int, unsigned int);
void cerrar_archivos(FILE**, int);
void verificarYAgregarElementos(heap_t*, unsigned int*,FILE**, int);
void procesar_archivos(heap_t* heap,unsigned int* contadores ,FILE** archivos, int cant, FILE* salida);
char* __crear_ruta(unsigned int, unsigned int);


char* merger(char** rutas, unsigned int i, unsigned int max,int cant, FILE* salida){
	FILE** archs = abrir_archivos(rutas, cant,i);
	heap_t* heap = heap_crear(comparacionRegistros);

	unsigned int contadores[cant];
	for (unsigned int j = 0; j < cant; j++) contadores[j] = 0;

	FILE* outfile = salida;
	char* ruta_aux = NULL;

	if (!outfile){
		ruta_aux = __crear_ruta(i,max);
		outfile = fopen(ruta_aux, escritura_archivos());
	}

	procesar_archivos(heap, contadores, archs, cant, outfile);
	cerrar_archivos(archs, cant);
	fclose(outfile);

	heap_destruir(heap,NULL);
	return ruta_aux;
}


int merger_MergearArchivos(char** rutas, int cant){
	if (cant <= 0) return MERGER_ERROR;


	FILE* archSalida = fopen (SALIDA, escritura_archivos());
	if (!archSalida) return MERGER_ERROR;
	unsigned int c = cant/CANT_ARCHIVOS_SEGUIDOS;

	//Primera etapa del merge. Subdivido por la constante CANT_ARCHIVOS_SEGUIDOS a tratar,
	//abro cada archivo, creo el arbol con un elemento de cada archivo, y voy avanzando en cada uno.
	char* rutas_aux[c];
	for (unsigned int i = 0; i <= c; i++){
		rutas_aux[i] = merger(rutas, i,c ,cant - i*c, NULL);
	}

	merger(rutas_aux, 0, c,c+1, archSalida);
	for (unsigned int j = 0; j <= c; j++){
		remove(rutas_aux[j]);
		free(rutas_aux[j]);
	}
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

void verificarYAgregarElementos(heap_t* heap, unsigned int* contadores,FILE** archivos, int cant){

	for (unsigned int i = 0; i < cant; i++){
		if (contadores[i] == 0){
			for (unsigned j = 0; j < CANT_REGISTROS_POR_ARCHIVO; j++){
				dato_t* dato = malloc (sizeof(dato_t));
				dato->numArchivo = i;
				dato->registro = registro_leer(archivos[i]);
				if (dato->registro != NULL)
					heap_encolar(heap, dato);
				else
					free(dato);
			}
			contadores[i] = CANT_REGISTROS_POR_ARCHIVO;
		}
	}
}

bool archivos_vacios(FILE** archivos, int cant){
	for (unsigned int i = 0; i < cant; i++)
		if (!feof(archivos[i])) return false;

	return true;
}

void procesar_archivos(heap_t* heap,unsigned int* contadores ,FILE** archivos, int cant, FILE* salida){
	while (!archivos_vacios(archivos, cant) || !heap_esta_vacio(heap)){
		verificarYAgregarElementos(heap,contadores,archivos, cant);
		dato_t* dato = heap_desencolar(heap);
		contadores[dato->numArchivo] = contadores[dato->numArchivo] - 1;
		registro_escribir(salida, dato->registro);
		registro_destruir(dato->registro);
		free(dato);
	}

}


/************************************************************************************/
// FUNCIONES ESPECIALES AUXILIARES

char* numeroToString(unsigned int num, unsigned int cantDigitos){
	char* dev = malloc(sizeof(char) * (cantDigitos + 1));
	dev[cantDigitos] = '\0';
	for (unsigned int i = (cantDigitos); i > 0; i--){
		dev[i-1] = num % 10 + '0';
		num /= 10;
	}

	return dev;
}

char* __crear_ruta(unsigned int num, unsigned int maximo){
	unsigned int cant = 0;
	while (maximo != 0){
		maximo /= 10;
		cant++;
	}
	if (cant == 0) cant = 1;

	char* ruta = malloc (sizeof(char) * (strlen(SALIDA_TEMPORAL) + 1 + cant));
	char* numeroCadena = numeroToString(num, cant);
	strcpy(ruta, numeroCadena);

	for (unsigned int i = strlen(numeroCadena); i < (strlen(numeroCadena)+strlen(SALIDA_TEMPORAL));i++){
		ruta[i] = SALIDA_TEMPORAL[i - strlen(numeroCadena)];
	}
	free(numeroCadena);
	ruta[strlen(SALIDA_TEMPORAL)+cant] = '\0';
	return ruta;
}

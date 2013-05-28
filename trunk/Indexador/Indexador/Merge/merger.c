#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "merger.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/TDAs/heap.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/registro.h"
#include "../../Carpetas Compartidas/Log/log.h"

#define CANT_ARCHIVOS_SEGUIDOS 15
#define CANT_REGISTROS_POR_ARCHIVO 200

const char* SALIDA_TEMPORAL = "tempmerge.jem";	//salida para archivos temporales

/*Definicion del tipo de datos a guardar dentro del heap para hacer de auxiliar en el merge*/


//ACLARACION: se hace un merge multi-etapa

FILE** abrir_archivos(char**, int,int);
void cerrar_archivos(FILE**, int);
void verificarYAgregarElementos(heap_t*, unsigned int*,FILE**, int);
void procesar_archivos(unsigned int* contadores ,FILE** archivos, int cant, FILE* salida, size_t*);
int copiarArchivo(const char* destino, const char* origen);

char* merger(char** rutas, unsigned int i, unsigned int max,int inicial,int cant, FILE* salida, size_t* ordenados){
	FILE** archs = abrir_archivos(rutas,inicial, cant);

	unsigned int contadores[cant];
	for (unsigned int j = 0; j < cant; j++) contadores[j] = 0;

	FILE* outfile = salida;
	char* ruta_aux = NULL;

	if (!outfile){
		ruta_aux = __crear_ruta(i,max, SALIDA_TEMPORAL);
		outfile = fopen(ruta_aux, escritura_archivos());
	}
	procesar_archivos(contadores, archs, cant, outfile, ordenados);
	cerrar_archivos(archs, cant);
	fclose(outfile);

	return ruta_aux;
}


int merger_MergearArchivos(char** rutas, int cant, const char* salida_final){
	if (cant <= 0) return MERGER_ERROR;

	if (cant == 1){
		emitir_impresion("Mergeando Archivos temporales", 0,4);
		return copiarArchivo(salida_final, rutas[0]);
	}

	FILE* archSalida = fopen (salida_final, escritura_archivos());
	if (!archSalida) return MERGER_ERROR;
	unsigned int c = cant/CANT_ARCHIVOS_SEGUIDOS; //c es la cantidad de pasadas a hacer - 1

	unsigned int corrector = 0;
	if (cant % CANT_ARCHIVOS_SEGUIDOS == 0)
		corrector = 1;
	c -= corrector;

	size_t ordenados = 0;
	//Primera etapa del merge. Subdivido por la constante CANT_ARCHIVOS_SEGUIDOS a tratar,
	//abro cada archivo, creo el arbol con un elemento de cada archivo, y voy avanzando en cada uno.
	char* rutas_aux[c];
	int cant_procesados = 0;
	log_emitir("Se inicia primera etapa del merge", LOG_ENTRADA_PROCESO);
	for (unsigned int i = 0; i <= c; i++){
		emitir_impresion("Mergeando Archivos Ordenados", ordenados, registro_totales());
		int a_procesar;
		if (cant_procesados + CANT_ARCHIVOS_SEGUIDOS <= cant){
			a_procesar = CANT_ARCHIVOS_SEGUIDOS;
		}else{
			a_procesar = cant - cant_procesados - 1;
		}
		rutas_aux[i] = merger(rutas, i,c ,cant_procesados,a_procesar, NULL, &ordenados);
		cant_procesados += a_procesar;
	}
	log_emitir("Finalizada primera etapa del merge", LOG_ENTRADA_PROCESO);

	emitir_impresion("Mergeando Archivos temporales", 0,4);
	log_emitir("Se inicia segunda etapa del merge", LOG_ENTRADA_PROCESO);
	merger(rutas_aux, 0, c,0,c+1, archSalida, &ordenados);
	log_emitir("Finalizada segunda etapa del merge", LOG_ENTRADA_PROCESO);

	log_emitir("Se remueven los archivos temporales de la primera etapa del merge", LOG_ENTRADA_PROCESO);
	for (unsigned int j = 0; j <= c; j++){
		remove(rutas_aux[j]);
		free(rutas_aux[j]);
	}
	return MERGER_OK;
}


FILE** abrir_archivos(char** rutas, int inicial, int cantidad){
	FILE** archivos = malloc (sizeof(FILE*)* cantidad);
	for (unsigned int j = 0; j < (cantidad);j++){
		archivos[j] = fopen(rutas[inicial+j], lectura_archivos());
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
				dato->freezado = false;
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


void procesar_archivos(unsigned int* contadores ,FILE** archivos, int cant, FILE* salida, size_t *ordenados){
	heap_t* heap = heap_crear(comparacionRegistros);
	while (!archivos_vacios(archivos, cant) || !heap_esta_vacio(heap)){
		verificarYAgregarElementos(heap,contadores,archivos, cant);
		dato_t* dato = heap_desencolar(heap);

		if (dato){
			contadores[dato->numArchivo] = contadores[dato->numArchivo] - 1;
			registro_escribir(salida, dato->registro);
			*ordenados = *ordenados + 1;
			registro_destruir(dato->registro);
			free(dato);
		}
	}
	heap_destruir(heap,NULL);
}

int copiarArchivo(const char* destino, const char* origen){
	FILE* org = fopen(origen, lectura_archivos());
	FILE* dst = fopen(destino, escritura_archivos());
	if (!org || !dst){
		fclose(org);
		fclose(dst);
		return MERGER_ERROR;
	}

	while (!feof(org)){
		char c = fgetc(org);
		fprintf(dst, "%c", c);
	}

	fclose(org);
	fclose(dst);
	return MERGER_OK;
}

/************************************************************************************/



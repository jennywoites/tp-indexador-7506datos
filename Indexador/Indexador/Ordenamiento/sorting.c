#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/TDAs/heap.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/registro.h"
#include "../../Carpetas Compartidas/Log/log.h"
#include "../Merge/merger.h"

//const char* SALIDA_TEMPORAL_SORT = "../../../../../../media/OpSys/Debug/tempsort.jem";
const char* SALIDA_TEMPORAL_SORT = "tempsort.jem";

#define SORT_CANT_REGISTROS 2000000
#define SORT_CANT_RUTAS_MAX 999


//El metodo a utilizar es el REPLACEMENT SELECTION

void sort_meterEnHeap(heap_t* heap, FILE* archEnt, unsigned long inicial){
	unsigned long i = 0;
	while ( (i+inicial) < SORT_CANT_REGISTROS && !feof(archEnt)){
		registro_t* reg = registro_leer(archEnt);
		dato_t* d = malloc(sizeof(dato_t));
		d->registro = reg;
		d->numArchivo = 0;
		d->freezado = false;
		if (reg != NULL)
			heap_encolar(heap, d);
		else
			free(d);
		i++;
	}
}

void sort_guardarHastaFreezados(heap_t* heap, FILE* archEnt,FILE* archSal, size_t* ordenados){
	dato_t* anterior = NULL;

	while (!heap_esta_vacio(heap) && !((dato_t*)heap_ver_max(heap))->freezado){

		dato_t* actual = heap_desencolar(heap);
		bool agregar = true;
		if (anterior){
			if (comparacionRegistros(anterior, actual) < 0){
				agregar = false;
				actual->freezado = true;
				heap_encolar(heap, actual);
			}
		}

		if (agregar){
			registro_escribir(archSal, actual->registro);
			*ordenados = *ordenados+1;
			if (anterior){
				registro_destruir(anterior->registro);
				free(anterior);
			}
			anterior = actual;

			registro_t* reg = registro_leer(archEnt);
			dato_t* d = malloc(sizeof(dato_t));
			d->registro = reg;
			d->numArchivo = 0;
			d->freezado = false;
			if (reg != NULL)
				heap_encolar(heap, d);
			else
				free(d);
		}

	}
	if (anterior){
		registro_destruir(anterior->registro);
		free(anterior);
	}
}

void sort_desfreezar(heap_t** heap){
	heap_t* heap_aux = heap_crear(comparacionRegistros);
	heap_t* heap_actual = *heap;
	while (!heap_esta_vacio(heap_actual)){
		dato_t* dato = heap_desencolar(heap_actual);
		if (!dato) continue;
		dato->freezado = false;
		heap_encolar(heap_aux, dato);
	}
	heap_destruir(heap_actual,NULL);
	*heap = heap_aux;
}


int sorting_ordenarArchivo(const char* entrada, const char* salida){
	heap_t* heap = heap_crear(comparacionRegistros);

	FILE* archEnt = fopen(entrada, lectura_archivos());
	unsigned int num = 0;
	char** rutas = malloc (sizeof(char*) * SORT_CANT_RUTAS_MAX);
	size_t ordenados = 0;
	log_emitir("Se Comienza el proceso de Ordenamiento del archivo parcedo", LOG_ENTRADA_PROCESO);
	while (!feof(archEnt) || !heap_esta_vacio(heap)){
		log_emitir("Se genera particion ordenada", LOG_ENTRADA_PROCESO);
		emitir_impresion("Ordenando Archivo Parseado", ordenados, registro_totales());
		sort_meterEnHeap(heap, archEnt, heap_cantidad(heap));
		char* ruta = __crear_ruta(num, SORT_CANT_RUTAS_MAX, SALIDA_TEMPORAL_SORT);
		FILE* archSal = fopen(ruta, escritura_archivos());
		rutas[num] = ruta;
		num++;
		sort_guardarHastaFreezados(heap, archEnt, archSal,&ordenados);
		fclose(archSal);
		sort_desfreezar(&heap);
	}
	fclose(archEnt);
	int ok = merger_MergearArchivos(rutas, num,salida);

	log_emitir("Se remueven los archivos temporales del proceso de ordenamiento", LOG_ENTRADA_PROCESO);
	for (unsigned int j = 0; j < num; j++){
		remove(rutas[j]);
		free(rutas[j]);
	}
	free(rutas);
	heap_destruir(heap,NULL);
	if ( ok == MERGER_OK)
		return SORTING_OK;
	else
		return SORTING_ERROR;
}

#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/TDAs/heap.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/registro.h"
#include "../../Carpetas Compartidas/Log/log.h"
#include "../Merge/merger.h"

const char* SALIDA_TEMPORAL_SORT = "tempsort.jem";


//POR AHORA LO HAGO CABEZA!!!!!!
int sorting_ordenarArchivo(const char* entrada, const char* salida){
	heap_t* heap = heap_crear(comparacionRegistros);

	FILE* archEnt = fopen(entrada, lectura_archivos());
	unsigned long i = 0;
	unsigned int num = 0;
	char** rutas = malloc (sizeof(char*) * 100);

	while (!feof(archEnt)){
		while ( i < 100000 && !feof(archEnt)){
			registro_t* reg = registro_leer(archEnt);
			dato_t* d = malloc(sizeof(dato_t));
			d->registro = reg;
			d->numArchivo = 0;
			if (reg != NULL)
				heap_encolar(heap, d);
			else
				free(d);
			i++;
		}
		i = 0;
		char* ruta = __crear_ruta(num, 999, SALIDA_TEMPORAL_SORT);
		FILE* archSal = fopen(ruta, escritura_archivos());
		rutas[num] = ruta;
		num++;

		while (!heap_esta_vacio(heap)){
			dato_t* d = heap_desencolar(heap);
			registro_t* reg = d->registro;
			registro_escribir(archSal,reg);
			registro_destruir(reg);
			free(d);
		}
		fclose(archSal);
	}
	fclose(archEnt);

	merger_MergearArchivos(rutas, num,salida);

	for (unsigned int j = 0; j < num; j++){
		remove(rutas[j]);
		free(rutas[j]);
	}
	free(rutas);
	heap_destruir(heap,NULL);
	return SORTING_OK;
}

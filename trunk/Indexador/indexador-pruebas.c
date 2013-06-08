#include <stdio.h>
#include "Indexador/Parseo/parserIndex.h"
#include "Indexador/Ordenamiento/sorting.h"
#include "Indexador/Indexado/indexer.h"
#include "Carpetas Compartidas/Log/log.h"
#include "Carpetas Compartidas/interaccion.h"
#include <stdlib.h>
#include <stdbool.h>
#include "Buscador/buscador.h"
#include "Buscador/resultado.h"
#include "Buscador/Parseo/parserQuery.h"
#include "Carpetas Compartidas/TDAs/lista.h"

#include <time.h>
#include <bits/time.h>

const char* SALIDA_PARSER = "parser.jem";
const char* SALIDA_SORT = "sort.jem";
const char* INDICE = "indice.jem";
const char* LEXICO = "lexico.jem";
const char* DIFERENTES = "diferentes.jem";
const char* NOMBRE_ARCHIVOS = "archs.jem";
const char* OFFSET_ARCHIVOS = "offarchs.jem";


void print_test(char* name, bool result){
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void buscar(){
	printf("Cargando el lexico en memoria, espere un instante\n");
	buscador_t* busq = buscador_crear(LEXICO,DIFERENTES);
	printf("Cargado el lexico\n");

	while (1){ //por ahora lo cortamos con ctrl+c, luego hago una funcion posta :P
		printf("Ingrese busqueda\n");
		char* query = leer_texto();

		clock_t tiempo_ini = clock();

		lista_t* busquedas = parserQuery_parsearConsulta(query);

		if (lista_largo(busquedas) > 1){
			resultado_t* resul = buscador_buscar(busq, busquedas,INDICE);
			lista_t* soluciones = resultado_realizarIntersecciones(resul);
			solucion_emitir(soluciones, NOMBRE_ARCHIVOS, OFFSET_ARCHIVOS);
			lista_destruir(soluciones, destructor_solucion);
			resultado_destruir(resul);
		}else{
			buscador_busquedaPuntual(busq, (char*)lista_ver_primero(busquedas),INDICE, NOMBRE_ARCHIVOS, OFFSET_ARCHIVOS);
		}
		clock_t tiempo = clock() - tiempo_ini;

		float segsTot = (float) tiempo / CLOCKS_PER_SEC;

		printf("Tiempo de busqueda: %f segundos\n", segsTot);

		lista_destruir(busquedas,free);
		free(query);
	}
	buscador_destruir(busq);

}

void indexar(){
	char** rutas;
	unsigned long cant;

	const char* directorio = "Textos_ejemplo_parseo";

	int aux = parserIndex_obtenerRutasDirectorios(directorio, &rutas, &cant);

	if (aux != PARSERINDEX_OK){
		printf ("Error al obtener los archivos\n");
		return;
	}

	aux = parserIndex_parsearArchivos(directorio, rutas,cant,SALIDA_PARSER, NOMBRE_ARCHIVOS, OFFSET_ARCHIVOS);

	if (aux == PARSERINDEX_OK){
		log_emitir("Paseo Realizado Correctamente", LOG_ENTRADA_INFORMATIVA_IMPORTANTE);
		aux = sorting_ordenarArchivo(SALIDA_PARSER, SALIDA_SORT);
		indexer_indexar(SALIDA_SORT, INDICE, LEXICO, DIFERENTES);

		remove(SALIDA_PARSER);
		remove(SALIDA_SORT);
	}else{
		log_emitir("Parseo incorrecto", LOG_ENTRADA_ERROR);
	}

	for (unsigned long i = 0; i < cant; i++){
		free(rutas[i]);
	}
	free(rutas);
	log_emitir("Se termino de Indexar los documentos", LOG_ENTRADA_INFORMATIVA_IMPORTANTE);
}


int main (int argc, char** argv){
	buscar();
	return 0;
}

#include <stdio.h>
#include "Indexador/Parseo/parserIndex.h"
#include "Indexador/Ordenamiento/sorting.h"
#include "Indexador/Indexado/indexer.h"
#include "Carpetas Compartidas/Log/log.h"
#include <stdlib.h>
#include <stdbool.h>
#include "Buscador/buscador.h"
#include "Carpetas Compartidas/TDAs/lista.h"

const char* SALIDA_PARSER = "parser.jem";
const char* SALIDA_SORT = "sort.jem";
const char* INDICE = "indice1.jem";
const char* LEXICO = "lexico1.jem";
const char* DIFERENTES = "diferentes1.jem";
const char* NOMBRE_ARCHIVOS = "archs1.jem";


void print_test(char* name, bool result){
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

int main (int argc, char** argv){

/*	char** rutas;
	unsigned long cant;

	const char* directorio = "Textos_ejemplo_parseo";

	int aux = parserIndex_obtenerRutasDirectorios(directorio, &rutas, &cant);

	if (aux != PARSERINDEX_OK){
		printf ("Error al obtener los archivos\n");
		return 0;
	}

	aux = parserIndex_parsearArchivos(directorio, rutas,cant,SALIDA_PARSER, NOMBRE_ARCHIVOS);

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
*/
	buscador_t* busq = buscador_crear(LEXICO,DIFERENTES);
	printf("Cargado el lexico\n");

	lista_t* busquedas = lista_crear();
//	lista_insertar_ultimo(busquedas, "KNOWN");
//	lista_insertar_ultimo(busquedas, "INTERNATIONALLY");
//	lista_insertar_ultimo(busquedas, "AS");
	//lista_insertar_ultimo(busquedas, "BANGKOK");
	//lista_insertar_ultimo(busquedas, "IS");
	//lista_insertar_ultimo(busquedas, "ALTHOUGH");
	lista_insertar_ultimo(busquedas, "THE");
	lista_insertar_ultimo(busquedas, "CAT");
	lista_insertar_ultimo(busquedas, "IS");
	lista_insertar_ultimo(busquedas, "UNDER");
	lista_insertar_ultimo(busquedas, "THE");
	lista_insertar_ultimo(busquedas, "TABLE");
	//lista_insertar_ultimo(busquedas, "IN");
	lista_insertar_ultimo(busquedas, "LALALA");
	//lista_insertar_ultimo(busquedas, "LALALA");
	//lista_insertar_ultimo(busquedas, "ARTHUR");
	//lista_insertar_ultimo(busquedas, "THE");
	//lista_insertar_ultimo(busquedas, "ANTHROPIC");
	//lista_insertar_ultimo(busquedas, "PRINCIPLE");

	resultado_t* resul = buscador_buscar(busq, busquedas,INDICE);
	resultado_emitirListado(resul,busquedas, NOMBRE_ARCHIVOS);

	resultado_destruir(resul);
	lista_destruir(busquedas,NULL);	
	buscador_destruir(busq);

	return 0;
}

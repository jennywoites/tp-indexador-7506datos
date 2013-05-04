#include "indexer.h"
#include <stdio.h>
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/registro.h"
#include "../../Carpetas Compartidas/Log/log.h"
#include "../../Carpetas Compartidas/TDAs/cola.h"
#define TAM_TERMINO_IMPOSIBLE 200


//void indexarEntrada(registro_t* actual, registro_t* anterior, FILE* archIndex, FILE* archLexico, unsigned long *frecDoc, unsigned long *frecPos, unsigned long* offset);

int indexer_indexar(const char* origen, const char* destino_index, const char* destino_lexico){
	FILE* archOrigen = fopen(origen, lectura_archivos());
	FILE* archIndice = fopen(destino_index, escritura_archivos());
	FILE* archLexico = fopen(destino_lexico, escritura_archivos());
	if (!archOrigen || !archIndice || !archLexico){
		fclose(archOrigen);
		fclose(archIndice);
		fclose(archLexico);
		return INDEXER_ERROR;
	}

	fprintf(archLexico,"REP\t\tDIST\t\tCARAC\t\t OFFSET\t\tFREC\n");

	lista_t* posiciones = lista_crear();
	lista_t* documentos = lista_crear();
	unsigned long offset = 0;
	registro_t* registro_anterior = NULL;

	log_emitir("Inicia el Indexado de archivos", LOG_ENTRADA_PROCESO);
	while (!feof(archOrigen)){
		registro_t* registro = registro_leer(archOrigen);
		if (!registro && !feof(archOrigen)){
			log_emitir("Lectura del archivo ordenado incorrecto", LOG_ENTRADA_ERROR);
			continue;
		}

		registro_escribirEnIndice(registro, registro_anterior, archIndice, archLexico, documentos, posiciones, &offset);

		if (registro_anterior)
			registro_destruir(registro_anterior);
		registro_anterior = registro;
	}
	if (registro_anterior)
		registro_destruir(registro_anterior);

	registro_escribirEnIndice(NULL, registro_anterior, archIndice, archLexico, documentos, posiciones, &offset);

	log_emitir("Finalizo el Indexado de archivos", LOG_ENTRADA_PROCESO);

	lista_destruir(documentos,NULL);
	lista_destruir(posiciones, NULL);
	fclose(archOrigen);
	fclose(archLexico);
	fclose(archIndice);
	return INDEXER_OK;
}

#include "levantador.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/termino.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/Log/log.h"
#include <stdlib.h>

void destructor(void* dato){
	termino_t* termino = (termino_t*) dato;
	termino_destruir(termino);
}


void cargarTrie(trie_t* trie, FILE* archFrontCoding, FILE* archDiferentes){
	log_emitir("Inicia la carga de terminos al TRIE", LOG_ENTRADA_PROCESO);
	termino_t* termino_anterior = NULL;
	while (!feof(archFrontCoding) && !feof(archDiferentes)){
		termino_t* termino = termino_leer(termino_anterior,archFrontCoding, archDiferentes);
		if (!termino) continue;
		char* cad = termino_obtenerPalabra(termino);
		if(!trie_guardar(trie, cad,termino))
			log_emitir("No se pudo almacenar un termino en el TRIE", LOG_ENTRADA_ERROR);
		free(cad);
		termino_anterior = termino;
	}
	log_emitir("Finalizo la carga de terminos al TRIE", LOG_ENTRADA_PROCESO);
}


trie_t* levantador_obtenerTrieLexico(const char* rutaFrontCoding, const char* rutaDiferentes){
	if (!rutaFrontCoding || !rutaDiferentes)
		return NULL;

	FILE* archFrontCoding = fopen(rutaFrontCoding, lectura_archivos());
	FILE* archDiferentes = fopen(rutaDiferentes, lectura_archivos());
	if (!archFrontCoding || !archDiferentes){
		fclose(archFrontCoding);
		fclose(archDiferentes);
		return NULL;
	}

	trie_t* trie = trie_crear(destructor);
	cargarTrie(trie, archFrontCoding, archDiferentes);
	fclose(archFrontCoding);
	fclose(archDiferentes);
	return trie;
}



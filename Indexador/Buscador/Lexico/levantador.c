#include "levantador.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/termino.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/debuffer.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/Log/log.h"
#include <stdlib.h>

#include <sys/stat.h>

/*void destructor(void* dato){
	termino_t* termino = (termino_t*) dato;
	termino_destruir(termino);
}*/


void cargarEstructura(void* contenedor,contenedor_guardar_dato_t funcion , FILE* archFrontCoding, FILE* archDiferentes, size_t cant_bytes){
	debuffer_t* debuff_frontcoding = debuffer_crear(archFrontCoding, cant_bytes);
	log_emitir("Inicia la carga de terminos al CONTENDOR", LOG_ENTRADA_PROCESO);
	termino_t* termino_anterior = NULL;
	termino_t* termino;
	while ( (termino = termino_leer(termino_anterior,debuff_frontcoding, archDiferentes)) /*&& !feof(archDiferentes)*/){
		//termino_t* termino = termino_leer(termino_anterior,debuff_frontcoding, archDiferentes);
		//if (!termino) continue;
		char* cad = termino_obtenerPalabra(termino);
		if(!funcion(contenedor, cad,termino))
			log_emitir("No se pudo almacenar un termino en el CONTENEDOR", LOG_ENTRADA_ERROR);
		free(cad);
		termino_anterior = termino;
	}
	log_emitir("Finalizo la carga de terminos al CONTENEDOR", LOG_ENTRADA_PROCESO);
	debuffer_destruir(debuff_frontcoding);
}


int levantador_obtenerContenedorLexico(void* contenedor,contenedor_guardar_dato_t funcion ,const char* rutaFrontCoding, const char* rutaDiferentes){
	if (!rutaFrontCoding || !rutaDiferentes)
		return LEVANTADOR_ERROR;

	FILE* archFrontCoding = fopen(rutaFrontCoding, lectura_archivos());
	FILE* archDiferentes = fopen(rutaDiferentes, lectura_archivos());
	if (!archFrontCoding || !archDiferentes){
		fclose(archFrontCoding);
		fclose(archDiferentes);
		return LEVANTADOR_ERROR;
	}

	struct stat st;
	stat(rutaFrontCoding, &st);
	size_t cant_bytes = st.st_size;
	cargarEstructura(contenedor, funcion, archFrontCoding, archDiferentes, cant_bytes);
	fclose(archFrontCoding);
	fclose(archDiferentes);
	return LEVANTADOR_OK;
}



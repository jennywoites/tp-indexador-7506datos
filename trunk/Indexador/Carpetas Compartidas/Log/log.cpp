#include "log.h"
#include <stdio.h>

const char* LOG_MSJ_INFORMATIVO = "INFORMATIVO";
const char* LOG_MSJ_INFORMATIVO_IMP = "INFORMATIVO IMPORTANTE";
const char* LOG_MSJ_ERROR = "ERROR";
const char* LOG_MSJ_PROCESO = "PROCESO";
const char* LOG_ARCHIVO = "log.jem";

void __realizar_impresion(std::string mensaje, unsigned int tipo){

	//ACA HABRIA QUE AGREGAR UN SEMAFORO
	FILE* LOG_SALIDA = fopen(LOG_ARCHIVO, "a");

	/*if (tipo == LOG_ENTRADA_INFORMATIVA)
		fprintf(LOG_SALIDA, "%s - ",LOG_MSJ_INFORMATIVO);
	else if (tipo == LOG_ENTRADA_INFORMATIVA_IMPORTANTE)
		fprintf(LOG_SALIDA, "%s - ",LOG_MSJ_INFORMATIVO_IMP);
	else if (tipo == LOG_ENTRADA_ERROR)
		fprintf(LOG_SALIDA, "%s - ",LOG_MSJ_ERROR);
	else if (tipo == LOG_ENTRADA_PROCESO)
		fprintf(LOG_SALIDA, "%s - ",LOG_MSJ_PROCESO);
	*/
	fprintf(LOG_SALIDA, "%s\n",mensaje.c_str());
	fclose(LOG_SALIDA);
	//ACA CERRAMOS EL SEMAFORO
}

void log_emitir(std::string mensaje, unsigned int tipo){
	switch(LOG_MODO){
	case LOG_NADA: return;
	case LOG_COMPLETO:
		__realizar_impresion(mensaje,tipo);
		return;
	case LOG_RELEASE:
		if (tipo == LOG_ENTRADA_INFORMATIVA_IMPORTANTE || tipo == LOG_ENTRADA_ERROR)
			__realizar_impresion(mensaje, tipo);
		return;
	case LOG_DEBUG_ERRORES:
		if (tipo == LOG_ENTRADA_ERROR)
			__realizar_impresion(mensaje, tipo);
		return;
	case LOG_DEBUG_ERRORES_Y_PROCESOS:
		if (tipo == LOG_ENTRADA_ERROR || tipo == LOG_ENTRADA_PROCESO)
			__realizar_impresion(mensaje, tipo);
		return;
	}
}


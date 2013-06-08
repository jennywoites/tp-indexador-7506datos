#include "parserQuery.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

void generarTermino(lista_t* terminos, char* buffer){
	__toUPPERCase(buffer);

	char* bufferSinEliminables = eliminarCaracteresPrescindibles(buffer, false);
	if (strlen(bufferSinEliminables) == 0){
		free(bufferSinEliminables);
		return;
	}

	unsigned int cantNum;
	char** bufferSepNum = separarSiSonNumeros(bufferSinEliminables, &cantNum);

	if (!bufferSepNum){
		lista_insertar_ultimo(terminos, bufferSinEliminables);
	}else{
		for (unsigned int k = 0; k < cantNum;k++){
			lista_insertar_ultimo(terminos, bufferSepNum[k]);
			free(bufferSepNum[k]);
		}
		free(bufferSepNum);
		free(bufferSinEliminables);
	}


}


lista_t* parserQuery_parsearConsulta(const char* query){
	if (strlen(query) == 0) return NULL;
	lista_t* terminos = lista_crear();
	unsigned int j = 0;
	char* buffer = malloc (sizeof(char) * (strlen(query)+1));
	char c;
	for (unsigned int i = 0; i <= strlen(query); i++){
		if (i < strlen(query))
			c = query[i];
		if (!caracterDeSeparacion(c) && i < strlen(query)){
			buffer[j] = c;
			j++;
		}else{
			buffer[j] = '\0';
			j = 0;
			generarTermino(terminos, buffer);
			free(buffer);
			buffer = malloc (sizeof(char) * (strlen(query)+1));
		}
	}
	free(buffer);
	return terminos;
}

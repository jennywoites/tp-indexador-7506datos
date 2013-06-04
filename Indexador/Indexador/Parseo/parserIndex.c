#include "parserIndex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/Log/log.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/registro.h"

//Para poder ver lo que haya en un directorio:
#include <dirent.h>
#define DT_DIR 4
#define DT_FILE 8

#define TAM 400

const char SEPARADORES[] = {' ', '-', '\n', '/', '_'};
unsigned int CANT_SEPARADORES = 5;
const char PRESCINDIBLES[] = {',' , ';' , '.' , '?' , '!', '\"', '\'', '^', '[',};
unsigned int CANT_PRESCINDIBLES = 7;


int parserIndex_obtenerParametros(int argc, char** argv,char** cadenas){
	if (argc != 3){
		//ver si se hace algo en caso de tener argumentos de mas o de menos
		return PARSERINDEX_ERROR;
	}

	cadenas[0] = malloc (sizeof(char) * (strlen(argv[1])+1));
	cadenas[1] = malloc (sizeof(char) * (strlen(argv[2])+1));

	strcpy(cadenas[0], argv[1]);
	strcpy(cadenas[1], argv[2]);

	return PARSERINDEX_OK;
}

int filtro (struct dirent* d){
	if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) return 0;

	//Por ahora, a los sub-directorios los descartamos (y cualquier cosa que no sea un archivo)
	if (d->d_type != DT_FILE) return 0;
	return 1;
}

int parserIndex_obtenerRutasDirectorios(const char* directorio, char*** rutas, unsigned long* cant){
	struct dirent** directorios = NULL;


	(*cant) = scandir(directorio, &directorios, filtro, NULL);
	//El tercer campo es una funcion de filtro que si devuelve 0, hara que se ignore
	//el directorio/archivo leido. El cuarto campo es una funcion de ordenamiento
	//(creo que se puede mandar una que se llama alphasort());

	if ((*cant) <= 0) return PARSERINDEX_ERROR;
	char** r = malloc (sizeof(char*) * (*cant));
	if (!rutas) return PARSERINDEX_ERROR;

	for (int i = 0; i < (*cant); i++){
		r[i] = malloc (sizeof(char)*(strlen(directorios[i]->d_name) + strlen(directorio)+2));
		sprintf(r[i], "%s/%s", directorio, directorios[i]->d_name);
		free(directorios[i]);
	}
	free(directorios);
	*rutas = r;
	return PARSERINDEX_OK;
}

/* **************************************************************************** */

char* eliminarCaracteresPrescindibles(char*, bool);
void tratarPalabra(char*, unsigned long, unsigned long, FILE*);
bool caracterDeSeparacion(char c);
void __toUPPERCase(char*);
char** separarSiSonNumeros(char*, unsigned int*);

int parserIndex_parsearArchivo(const char* ruta_archivo, unsigned long num, FILE* salida){
	FILE* arch = fopen(ruta_archivo, lectura_archivos());
	if (!arch) return PARSERINDEX_ERROR;
	unsigned int i;
	unsigned int tam = TAM;
	unsigned long pos = 1;
	while (!feof(arch)){
		char* buffer = malloc (sizeof(char) * TAM);
		i = 0;
		char c;
		while (lectura_anticipada(arch, &c) && !caracterDeSeparacion(c)){
			if (i == tam){
				tam += TAM;
				buffer = realloc (buffer, sizeof(char)*tam);
			}
			buffer[i] = c;
			i++;
		}
		if (i == tam)
			buffer = realloc (buffer, sizeof(char)*(tam+1));
		buffer[i] = '\0';

		__toUPPERCase(buffer);

		char* bufferSinEliminables = eliminarCaracteresPrescindibles(buffer, false);
		free(buffer);
		if (strlen(bufferSinEliminables) == 0){
			free(bufferSinEliminables);
			continue;
		}

		unsigned int cantNum;
		char** bufferSepNum = separarSiSonNumeros(bufferSinEliminables, &cantNum);

		if (!bufferSepNum){
			tratarPalabra(bufferSinEliminables, num, pos, salida);
			pos++;
		}else{
			for (unsigned int k = 0; k < cantNum;k++){
				tratarPalabra(bufferSepNum[k], num, pos, salida);
				pos++;
				free(bufferSepNum[k]);
			}
			free(bufferSepNum);
		}

		free(bufferSinEliminables);
	}

	fclose(arch);
	return PARSERINDEX_OK;
}


bool caracterDeSeparacion(char c){
	for (unsigned int i = 0; i < CANT_SEPARADORES; i++)
		if (c == SEPARADORES[i]) return true;

	return false;
}

bool caracterPrescindible(char c){
	/*
	 for (unsigned int i = 0; i < CANT_PRESCINDIBLES)
	  	  if (c == PRESCINIBLES[i]) return true;
	 return false;
	 */

	if (c == '@') return false;
	if (c == '&') return false;
	if (c == '_') return false;
	if (c < '0') return true;
	if (c > '9' && c < 'A') return true;
	if (c > 'Z' && c < 'a') return true;
	if (c > 'z') return true;
	return false;
}

char* eliminarCaracteresPrescindibles(char* cadena, bool duplicante){
	char* nueva = malloc (sizeof(char)* (strlen(cadena)+1));
	unsigned int cant = 0;
	for (unsigned int i = 0; i < strlen(cadena); i++){
		bool elim = false;
		elim = caracterPrescindible(cadena[i]);

		if(!elim){
				nueva[cant] = cadena[i];
				cant++;
		}
	}
	nueva[cant] = '\0';
	return nueva;
}


void tratarPalabra(char* palabra, unsigned long doc, unsigned long pos, FILE* salida){
	//Por ahora voy a imprimir simplemente:
	fprintf(salida,"%s;%lu;%lu\n", palabra, doc, pos);
	registro_aumentarCantidad();
}

void __toUPPERCase(char* cadena){
	for (unsigned int i = 0; i < strlen(cadena); i++){
		char c = cadena[i];
		if (c <= 'z' && c >= 'a'){
			cadena[i] = c + ('A' - 'a');
		}
	}
}

bool sonTodosNumeros(char* cad){
	for (unsigned int i = 0; i < strlen(cad); i++)
		if (cad[i] < '0' || cad[i] > '9') return false;
	return true;
}

char** separarSiSonNumeros(char* buffer, unsigned int* cant){
	if (!sonTodosNumeros(buffer)) return NULL;

	char** numeritos = malloc (sizeof(char*) * (strlen(buffer)));
	*cant = strlen(buffer);

	for (unsigned int i = 0; i < strlen(buffer); i++){
		numeritos[i] = malloc (sizeof(char) * 2);
		(numeritos[i])[0] = buffer[i];
		(numeritos[i])[1] = '\0';
	}

	return numeritos;
}


int comprimirNombres(char* directorio, char** rutas_archivos, unsigned long cant, const char* salida);

int parserIndex_parsearArchivos(char* directorio, char** rutas_archivos, unsigned long num, const char* ruta_salida, const char* salida_nombres){
	if (num == 0) return PARSERINDEX_OK;

	FILE* arch = fopen(ruta_salida, escritura_archivos());
	if (!arch) return PARSERINDEX_ERROR;

	bool ok = true;
	log_emitir("Inicia Parseo de archivos", LOG_ENTRADA_PROCESO);
	for (unsigned long i = 0; i < num; i++){
		emitir_impresion("Parseando Archivos",i,num);
		ok = parserIndex_parsearArchivo(rutas_archivos[i], i+1, arch) == PARSERINDEX_OK;
	}
	log_emitir("Finalizado Parseo de archivos",LOG_ENTRADA_PROCESO);
	fclose(arch);

	int aux = comprimirNombres(directorio, rutas_archivos, num, salida_nombres);

	if (ok && (aux == PARSERINDEX_OK)) return PARSERINDEX_OK;
	return PARSERINDEX_ERROR;
}

int comprimirNombres(char* directorio, char** rutas_archivos, unsigned long cant, const char* salida){
	//Por ahora solo los imprimo de cabeza:
	FILE* nombres = fopen(salida, escritura_archivos());
	if (!nombres) return PARSERINDEX_ERROR;

	unsigned int pos = strlen(directorio) + 1; //para evitar la primera barra
	fprintf(nombres, "%s/\n", directorio);

	log_emitir("Inicia la compresion de rutas de archivos", LOG_ENTRADA_PROCESO);
	for (unsigned long i = 0; i < cant; i++){
		fprintf(nombres, "%s", (rutas_archivos[i] + sizeof(char) * pos));
		if (i != (cant-1)) fprintf(nombres, "\n");
	}
	log_emitir("Finalizo la compresion de rutas de archivos", LOG_ENTRADA_PROCESO);
	fclose(nombres);
	return PARSERINDEX_OK;
}

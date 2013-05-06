#include "funcionesGeneralesArchivos.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#define MAXIMO_LINEAS 300
#define MAX_LONG_LINEA 300

const char* LECTURA = "r";
const char* ESCRITURA = "w";
bool lectura_anticipada(FILE* arch, char* c){
	if (feof(arch)) return false;
	(*c) = fgetc (arch);
	if ((*c)==EOF) return false;
	return true;
}


const char* lectura_archivos(){
	return LECTURA;
}
const char* escritura_archivos(){
	return ESCRITURA;
}

char** obtenerLineas(FILE* archivo, unsigned int* cant){
	char** lineas = malloc (sizeof(char*) * MAXIMO_LINEAS);

	char c;
	unsigned int i, j = 0;

	while (!feof(archivo)){
		char* buffer = malloc (sizeof(char) * MAX_LONG_LINEA);
		i = 0;
		do{
			c = fgetc(archivo);
			if (c == '\n')
				c = '\0';
			buffer[i] = c;
			i++;
		}while (!feof(archivo) && c != '\0' && c != '\n');

		if (feof(archivo)){free(buffer); continue;}

		lineas[j] = buffer;
		j++;
	}

	*cant = j;
	return lineas;
}

void verificarYAumentarTam(char** buffer, unsigned int* tam, unsigned int pos){
	if ((*tam) != pos) return;

	(*tam) = (*tam) + MAX_LONG_LINEA;

	char* aux = realloc (*buffer, sizeof(char) * (MAX_LONG_LINEA));
	if (!aux) return;

	*buffer = aux;
}


char* obtenerLinea(FILE* archivo){
	if (feof(archivo)) return NULL;

	unsigned int tam = MAX_LONG_LINEA;
	char* buffer = malloc (sizeof(char) * tam);
	unsigned int i = 0;
	char c = fgetc(archivo);

	while (c != '\n' && c != EOF){
		verificarYAumentarTam(&buffer, &tam, i);

		buffer[i] = c;
		i++;
		c = fgetc(archivo);
	}

	verificarYAumentarTam(&buffer, &tam, i);
	buffer[i] = '\0';
	return buffer;
}

char* numeroToString(unsigned int num, unsigned int cantDigitos){
	char* dev = malloc(sizeof(char) * (cantDigitos + 1));
	dev[cantDigitos] = '\0';
	for (unsigned int i = (cantDigitos); i > 0; i--){
		dev[i-1] = num % 10 + '0';
		num /= 10;
	}

	return dev;
}

char* __crear_ruta(unsigned int num, unsigned int maximo, const char* salida_temporal){
	unsigned int cant = 0;
	while (maximo != 0){
		maximo /= 10;
		cant++;
	}

	if (cant == 0) cant = 1;

	char* ruta = malloc (sizeof(char) * (strlen(salida_temporal) + 1 + cant));
	char* numeroCadena = numeroToString(num, cant);
	strcpy(ruta, numeroCadena);

	for (unsigned int i = strlen(numeroCadena); i < (strlen(numeroCadena)+strlen(salida_temporal));i++){
		ruta[i] = salida_temporal[i - strlen(numeroCadena)];
	}
	free(numeroCadena);
	ruta[strlen(salida_temporal)+cant] = '\0';
	return ruta;
}


void emitir_impresion(const char* mensaje,unsigned long num, unsigned long freq){

	unsigned long div0, div1, div2, div3;
	div1 = freq / 4;
	div2 = 2 * freq / 4;
	div3 = 3 * freq / 4;
	div0 = 0;

	if (num % freq == div0){
		system ("exec clear");
		printf("%s\n",mensaje);
		return;
	}

	if (num % freq == div1){
		system ("exec clear");
		printf("%s.\n",mensaje);
		return;
	}

	if (num % freq == div2){
		system ("exec clear");
		printf("%s..\n",mensaje);
		return;
	}


	if (num % freq == div3){
		system ("exec clear");
		printf("%s...\n",mensaje);
		return;
	}
}

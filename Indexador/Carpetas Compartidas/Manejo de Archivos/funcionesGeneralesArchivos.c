#include "funcionesGeneralesArchivos.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#define MAXIMO_LINEAS 300
#define MAX_LONG_LINEA 300

const char* LECTURA = "r";
const char* ESCRITURA = "w";

const char SEPARADORES[] = {' ', '-', '\n', '/', '_'};
unsigned int CANT_SEPARADORES = 5;
const char PRESCINDIBLES[] = {',' , ';' , '.' , '?' , '!', '\"', '\'', '^', '[',};
unsigned int CANT_PRESCINDIBLES = 7;

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


void emitir_impresion(const char* mensaje,size_t num, size_t total){

/*	unsigned long div0, div1, div2, div3;
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
	}*/
	system ("exec clear");
	float porc = ((float) num )/ total;
	porc *= 100;
	printf("%s : %f%% Completado \n", mensaje, porc);
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

char* __obtenerNombreDoc(const char* paths, const char* offsets,  size_t num){
	FILE* arch = fopen(paths, lectura_archivos());
	FILE* off = fopen(offsets, lectura_archivos());
	if (!arch || !off){
		fclose(arch);
		fclose(off);
		return NULL;
	}

	char* encabezado = obtenerLinea(arch);


	fseek(off,sizeof(size_t)*(num-1), SEEK_SET);
	size_t pos;
	size_t posSiguiente;


	fread(&pos, sizeof(size_t),1,off);
	fread(&posSiguiente, sizeof(size_t),1,off);

	fseek(arch, sizeof(char) * pos, SEEK_SET);
	char* relativa = malloc (sizeof(char) * (posSiguiente - pos + 1));

	fgets(relativa,posSiguiente - pos + 1,arch);

	char* ruta = malloc (sizeof(char) * (strlen(encabezado) + strlen(relativa) + 1));
	for (size_t i = 0; strlen(encabezado)> i;i++){
		ruta[i] = encabezado[i];
	}
	for (size_t i = strlen(encabezado); strlen(encabezado) + strlen(relativa)> i;i++){
		ruta[i] = relativa[i - strlen(encabezado)];
	}
	ruta[strlen(encabezado) + strlen(relativa)] = '\0';
	free(encabezado);
	free(relativa);
	fclose(arch);
	fclose(off);
	return ruta;
}

lista_t* complementarLista(lista_t* org, size_t cant){
	lista_t* cmpt = lista_crear();
	size_t* doc = lista_borrar_primero(org);
	for (size_t i = 1; i <= cant; i++){
		if (!doc || i != *doc){
			size_t* valor = malloc (sizeof(size_t));
			*valor = i;
			lista_insertar_ultimo(cmpt, valor);
		}else{
			if (doc) free(doc);
			doc = lista_borrar_primero(org);
		}
	}
	return cmpt;
}

size_t obtenerCantDocumentos(const char* ruta_tamanios, size_t doc){
	FILE* arch = fopen(ruta_tamanios, lectura_archivos());
	if (!arch) {
		return 0;
	}

	size_t cant;
	fseek(arch, sizeof(size_t) * (doc-1), SEEK_SET);
	fread(&cant, sizeof(size_t),1,arch);

	fclose(arch);
	return cant;
}

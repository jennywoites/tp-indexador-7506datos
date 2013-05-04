#include "registro.h"
#include <stdlib.h>
#include "funcionesGeneralesArchivos.h"
#include <string.h>
#include "../TDAs/cola.h"

struct registro{
	char* termino;
	unsigned long posicion;
	unsigned long documento;
};

int comparacionRegistros(const void* a, const void* b){
	dato_t* d1 = (dato_t*) a;
	dato_t* d2 = (dato_t*) b;

	//Si son terminos distintos ordeno alfanumericamente
	if (strcmp(d2->registro->termino, d1->registro->termino) != 0)
		return strcmp(d2->registro->termino, d1->registro->termino);

	//Mismo termino:
	if (d2->registro->documento != d1->registro->documento)
		return d2->registro->documento - d1->registro->documento;

	return d2->registro->posicion - d1->registro->posicion;
}

registro_t* registro_crear(const char* termino, unsigned long doc, unsigned long pos ){
	registro_t* reg = malloc (sizeof(registro_t));
	if (!reg) return NULL;

	reg->termino = malloc (sizeof(char) * (strlen(termino)+1));
	strcpy(reg->termino, termino);

	reg->documento = doc;
	reg->posicion = pos;
	return reg;
}

unsigned long stringToLong(char*);

registro_t* registro_leer(FILE* entrada){
	char* linea = obtenerLinea(entrada);
	if (!linea) return NULL;

	if (strlen(linea) < 2) {
		free(linea);
		return NULL;
	}

	char* termino = strtok(linea, ";");
	char* doc = strtok (NULL, ";");
	char* pos = strtok(NULL, ";");

	unsigned long documento = stringToLong(doc);
	unsigned long posicion = stringToLong(pos);

	registro_t* reg = registro_crear(termino, documento, posicion);
	free(linea);
	return reg;
}

void registro_escribir(FILE* salida, registro_t* reg){
	if (!reg) return;
	fprintf(salida, "%s;%lu;%lu\n",reg->termino, reg->documento, reg->posicion);
	//printf("TERMINO %s DOCUMENTO %lu POSICION %lu\n", reg->termino, reg->documento, reg->posicion);
}

void registro_destruir(registro_t* reg){
	if (!reg) return;
	free (reg->termino);
	free(reg);
}

unsigned long stringToLong(char* num){
	unsigned long n = 0;
	unsigned mult = 1;

	for ( int i = strlen(num) - 1; i >= 0 ; i--){
		char c = num[i];
		unsigned int digito = c - '0';
		n += digito * mult;
		mult *= 10;
	}
	return n;
}

void cerrar_documento(FILE* archIndice,lista_t* documentos, lista_t* posiciones_x_documentos, unsigned long *offset, unsigned long* cantidad);

void cerrar_termino(FILE* archLexico, unsigned long cant);

void crear_termino(FILE* archLexico, registro_t* actual,registro_t* anterior, unsigned long offset);

void registro_escribirEnIndice(registro_t* actual, registro_t* anterior, FILE* archIndice, FILE* archLexico, lista_t* documentos, lista_t* posiciones_x_documento, unsigned long* offset){
	unsigned long offsetActual = 0;
	bool agregarTermino = true;
	if (anterior && actual)
		agregarTermino = strcmp(actual->termino, anterior->termino) != 0;

	if (agregarTermino){
		if (anterior){
			unsigned long cantidad;
			cerrar_documento(archIndice, documentos, posiciones_x_documento, &offsetActual, &cantidad);
			cerrar_termino(archLexico, cantidad);
		}
		if (!actual) return;

		crear_termino(archLexico, actual, anterior, offsetActual);
	}

	if (!anterior || agregarTermino || actual->documento != anterior->documento){
		unsigned long *pos = malloc (sizeof(unsigned long));
		unsigned long *doc = malloc (sizeof(unsigned long));
		*pos = actual->posicion;
		*doc = actual->documento;

		lista_insertar_ultimo(documentos,doc);
		cola_t* aux = cola_crear();
		cola_encolar(aux, pos);
		lista_insertar_ultimo(posiciones_x_documento, aux);
		return;
	}
	//else:
	if (actual->documento == anterior->documento){
		unsigned long *pos = malloc (sizeof(unsigned long));
		*pos = actual->posicion;
		cola_t* aux = lista_ver_ultimo(posiciones_x_documento);
		cola_encolar(aux, pos);
		return;
	}
}


//FUNCIONES QUE AGREGAN LOS TERMINOS, ETC.....
//POR AHORA LAS HAGO A LO CABEZA

void cerrar_termino(FILE* archLexico, unsigned long cant){
	//compresor_comprimirFrecuencia(archLexico, cant);
	fprintf(archLexico,"%lu\n", cant);
}

void crear_termino(FILE* archLexico, registro_t* actual, registro_t* anterior, unsigned long offset){
	unsigned int repetidos = 0;
	bool ok = true;
	if (anterior){
		for (repetidos = 0; repetidos < strlen(anterior->termino) && ok; repetidos++){
			ok = ((actual->termino[repetidos]) == (anterior->termino[repetidos]));
		}
		if (!ok)
			repetidos--;
	}

	//compresor_comprimirRepetidosYDistinto(archLexico, repetidos, strlen(actual->termino) - repetidos);
	fprintf(archLexico,"%u\t\t%u\t\t", repetidos, strlen(actual->termino) - repetidos);

	for (unsigned int i = repetidos; i < strlen(actual->termino);i++){
		//compresor_comprimirCaracter(archLexico, actual->termino[i]); todo
		fprintf(archLexico, "%c", actual->termino[i]);
	}

	//compresor_comprimirOffset(archLexico, offset); todo
	fprintf(archLexico, "\t\t%lu\t\t", offset);
}


void cerrar_documento(FILE* archIndice,lista_t* documentos, lista_t* posiciones_x_documentos, unsigned long *offset, unsigned long* cantidad){
	*cantidad = lista_largo(documentos);

	unsigned long docAnterior = 0;
	while (!lista_esta_vacia(documentos)){
		unsigned long* docActual = lista_borrar_primero(documentos);

		int difOffset = 1;
		//difOffset = compresor_comprimirDocumento(archIndice, *posActual - posAnterior); todo
		fprintf(archIndice, "%lu ", *docActual - docAnterior);

		docAnterior = *docActual;
		*offset = (*offset) + difOffset;
		free(docActual);
		cola_t* posiciones = lista_borrar_primero (posiciones_x_documentos);

		//compresor_comprimirFrecuencia(archIndice, cola_cantidad(posiciones)); todo
		fprintf(archIndice,"%lu ", (unsigned long)cola_largo(posiciones));

		unsigned long posAnterior = 0;
		while (!cola_esta_vacia(posiciones)){
			unsigned long* posActual = cola_desencolar(posiciones);

			//compresor_comprimirPosicion(archIndice, *posActual - posAnterior); todo
			fprintf(archIndice, "%lu ", *posActual - posAnterior);

			posAnterior = *posActual;
			*offset = (*offset) + 1;
			free(posActual);
		}
		cola_destruir(posiciones,NULL);
	}
}

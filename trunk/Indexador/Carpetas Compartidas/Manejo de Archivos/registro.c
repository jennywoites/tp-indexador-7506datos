#include "registro.h"
#include <stdlib.h>
#include "funcionesGeneralesArchivos.h"
#include <string.h>
#include "../TDAs/cola.h"
#include "../Codigos/trasbordoCodigo.h"

struct registro{
	char* termino;
	unsigned long posicion;
	unsigned long documento;
};

size_t cant_registros = 0;

int comparacionRegistros(const void* a, const void* b){
	dato_t* d1 = (dato_t*) a;
	dato_t* d2 = (dato_t*) b;

	if (d1->freezado) return -1;
	if (d2->freezado) return 1;
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

size_t cerrar_punterosTermino(buffer_t* buffIndice,lista_t* documentos, lista_t* posiciones_x_documentos, unsigned long *offset, unsigned long* cantidad, size_t cantidad_docs);

void cerrar_termino(buffer_t* buffFrontCoding, unsigned long cant, size_t b);

void crear_termino(buffer_t* buffFrontCoding, FILE* archDiferentes, registro_t* actual,registro_t* anterior, unsigned long offset);

void registro_escribirEnIndice(registro_t* actual, registro_t* anterior, buffer_t* buffIndice, buffer_t* buffFrontCoding, FILE* archDiferentes, lista_t* documentos, lista_t* posiciones_x_documento, size_t cant_documentos){
	unsigned long offsetActual = 0;
	bool agregarTermino = true;
	if (anterior && actual)
		agregarTermino = strcmp(actual->termino, anterior->termino) != 0;

	if (agregarTermino){
		if (anterior){
			unsigned long cantidad;
			size_t b;
			b = cerrar_punterosTermino(buffIndice, documentos, posiciones_x_documento, &offsetActual, &cantidad, cant_documentos);
			cerrar_termino(buffFrontCoding, cantidad, b);
		}else{
			codificador_codificarDelta(buffIndice, cant_documentos);
			offsetActual = buffer_obtener_contador(buffIndice);
			buffer_reset_contador(buffIndice);
		}
		if (!actual) return;

		crear_termino(buffFrontCoding, archDiferentes, actual, anterior, offsetActual);
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

void cerrar_termino(buffer_t* buffFrontCoding, unsigned long cant, size_t b){
	//compresor_comprimirFrecuencia(archLexico, cant);
	//codificador_codificarDelta(buffFrontCoding,cant);
	comprimir_FrecuenciaDocumentos(buffFrontCoding,cant);
	comprimir_BGolomb(buffFrontCoding, b);
	//fprintf(archFrontCoding,"%lu\n", cant);
}

void crear_termino(buffer_t* buffFrontCoding, FILE* archDiferentes, registro_t* actual,registro_t* anterior, unsigned long offset){
	unsigned int repetidos = 0;
	bool ok = true;
	if (anterior){
		for (repetidos = 0; repetidos < strlen(anterior->termino) && ok; repetidos++){
			ok = ((actual->termino[repetidos]) == (anterior->termino[repetidos]));
		}
		if (!ok)
			repetidos--;
	}

	//codificador_codificarGamma(buffFrontCoding,repetidos+1);
	comprimir_LexicoRepetidos(buffFrontCoding, repetidos+1);
	//codificador_codificarGamma(buffFrontCoding,strlen(actual->termino) - repetidos);
	comprimir_LexicoDiferentes(buffFrontCoding, strlen(actual->termino) - repetidos);
	
	for (unsigned int i = repetidos; i < strlen(actual->termino);i++){
		//compresor_comprimirCaracter(archLexico, actual->termino[i]);
		fprintf(archDiferentes, "%c", actual->termino[i]);
	}

	//compresor_comprimirOffset(archLexico, offset);
	//TODO: ver de no comprimir el primer offset, y sacar ese +1
	//codificador_codificarDelta(buffFrontCoding,offset+1);
	comprimir_LexicoOffset(buffFrontCoding, offset);
	//fprintf(archFrontCoding, ";%lu;", offset);
}

size_t cerrar_punterosTermino(buffer_t* buffIndice,lista_t* documentos, lista_t* posiciones_x_documentos, unsigned long *offset, unsigned long* cantidad, size_t cant_docs){
	*cantidad = lista_largo(documentos);

	float p = (float) lista_largo(documentos) / cant_docs;
	size_t b = calcular_B_optimo(p);

	bool borrar = false;
	if (p > 0.5){
		documentos = complementarLista(documentos, cant_docs);
		borrar = true;
		p = 1-p;
	}

	unsigned long docAnterior = 0;
	while (!lista_esta_vacia(documentos)){
		unsigned long* docActual = lista_borrar_primero(documentos);
		comprimir_IndiceDistanciaDocumentos(buffIndice, *docActual - docAnterior, b, p);
		docAnterior = *docActual;
		free(docActual);
	}

	for (size_t i = 0; i < *cantidad; i++){
		cola_t* posiciones = lista_borrar_primero (posiciones_x_documentos);
		comprimir_FrecuenciaPosiciones(buffIndice,(unsigned long)cola_largo(posiciones));

		unsigned long posAnterior = 0;
		while (!cola_esta_vacia(posiciones)){
			unsigned long* posActual = cola_desencolar(posiciones);
			comprimir_IndiceDistanciaPosiciones(buffIndice,   *posActual - posAnterior);
			posAnterior = *posActual;
			free(posActual);
		}
		cola_destruir(posiciones,NULL);

	}

	/*
	while (!lista_esta_vacia(documentos)){
		unsigned long* docActual = lista_borrar_primero(documentos);

		//difOffset = compresor_comprimirDocumento(archIndice, *posActual - posAnterior);
		//codificador_codificarGamma(buffIndice,  *docActual - docAnterior);
		comprimir_IndiceDistanciaDocumentos(buffIndice, *docActual - docAnterior, b);
		//fprintf(archIndice, "%lu ", *docActual - docAnterior);
		
//		int difOffset = buffer_obtener_contador(buffIndice);
//		buffer_reset_contador(buffIndice);
		

		docAnterior = *docActual;
		// *offset = (*offset) + difOffset;
		free(docActual);
		cola_t* posiciones = lista_borrar_primero (posiciones_x_documentos);

		comprimir_FrecuenciaPosiciones(buffIndice,(unsigned long)cola_largo(posiciones));
		//codificador_codificarDelta(buffIndice,   (unsigned long)cola_largo(posiciones));
		//fprintf(archIndice,"%lu ", (unsigned long)cola_largo(posiciones));

		unsigned long posAnterior = 0;
		while (!cola_esta_vacia(posiciones)){
			unsigned long* posActual = cola_desencolar(posiciones);

			comprimir_IndiceDistanciaPosiciones(buffIndice,   *posActual - posAnterior);
			//codificador_codificarDelta(buffIndice,   *posActual - posAnterior);
			//fprintf(archIndice, "%lu ", *posActual - posAnterior);

			posAnterior = *posActual;
			// *offset = (*offset) + 1;
			free(posActual);
		}
		cola_destruir(posiciones,NULL);
	}
	*/
	int difOffset = buffer_obtener_contador(buffIndice);
	buffer_reset_contador(buffIndice);
	*offset = (*offset) + difOffset;
	if (borrar) lista_destruir(documentos, free);
	return b;
}

size_t registro_totales(){
	return cant_registros;
}

void registro_aumentarCantidad(){
	cant_registros++;
}

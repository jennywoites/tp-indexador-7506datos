#include <stdio.h>
#include "Indexador/Parseo/parserIndex.h"
#include "Indexador/Ordenamiento/sorting.h"
#include "Indexador/Indexado/indexer.h"
#include "Carpetas Compartidas/Log/log.h"
#include "Carpetas Compartidas/interaccion.h"
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "Buscador/buscador.h"
#include "Buscador/resultado.h"
#include "Buscador/Parseo/parserQuery.h"
#include "Carpetas Compartidas/TDAs/lista.h"
#include "Carpetas Compartidas/Codigos/trasbordoCodigo.h"

#include <time.h>
#include <bits/time.h>

#define OPC_INDEXAR 1
#define OPC_BUSCAR 2
#define OPC_ERROR -1
#define RUTA_DEFAULT "Textos_ejemplo_parseo"
#define OPC_IMPRIMIR_AYUDA 3
#define OPC_IMPRIMIR_VERSION 4

/*
const char* SALIDA_PARSER = "../../../../../../media/OpSys/Debug/parser.jem";
const char* SALIDA_SORT = "../../../../../../media/OpSys/Debug/sort.jem";
const char* INDICE = "../../../../../../media/OpSys/Debug/indice.jem";
const char* LEXICO = "../../../../../../media/OpSys/Debug/lexico.jem";
const char* DIFERENTES = "../../../../../../media/OpSys/Debug/diferentes.jem";
const char* NOMBRE_ARCHIVOS = "../../../../../../media/OpSys/Debug/archs.jem";
const char* OFFSET_ARCHIVOS = "../../../../../../media/OpSys/Debug/offarchs.jem";
const char* POSICIONES_ARCHIVOS = "../../../../../../media/OpSys/Debug/cantPosArchs.jem";
*/

const char* SALIDA_PARSER = "parser.jem";
const char* SALIDA_SORT = "sort.jem";
const char* INDICE = "indice.jem";
const char* LEXICO = "lexico.jem";
const char* DIFERENTES = "diferentes.jem";
const char* NOMBRE_ARCHIVOS = "archs.jem";
const char* OFFSET_ARCHIVOS = "offarchs.jem";
const char* POSICIONES_ARCHIVOS = "cantPosArchs.jem";


void print_test(char* name, bool result){
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

size_t obtenerCantidadDocs(const char* ruta){
	FILE* arch = fopen(ruta, lectura_archivos());
	debuffer_t* debuff = debuffer_crear(arch, 80);
	size_t c = decodificador_decodificarDelta(debuff);
	debuffer_destruir(debuff);
	fclose(arch);
	return c;
}

void definir_nombres_archivos(char* directorio,char* indice,char* lexico,char* diferentes){
	indice = __crear_ruta_repo(directorio, "INDICE");
	lexico = __crear_ruta_repo(directorio, "LEXICO");
	diferentes = __crear_ruta_repo(directorio, "DIFERENTES");
}


void realizar_busqueda(size_t cant, buscador_t* busq){

		printf("Ingrese busqueda\n");
		char* query = leer_texto();
		if (strlen(query) == 0){
			free(query);
			printf("Su busqueda esta vacia. \n");
			return;
		}

		if (strcmp(query, "salir") == 0){
			free(query);
			return;
		}

		clock_t tiempo_ini = clock();

		lista_t* busquedas = parserQuery_parsearConsulta(query);
		lista_iter_t* iter = lista_iter_crear(busquedas);
		while (!lista_iter_al_final(iter)){
			printf("%s\n", (char*)lista_iter_ver_actual(iter));
			lista_iter_avanzar(iter);}
		lista_iter_destruir(iter);

		if (lista_largo(busquedas) > 1){
			resultado_t* resul = buscador_buscar(busq, busquedas,INDICE,POSICIONES_ARCHIVOS);
			lista_t* soluciones = resultado_realizarIntersecciones(resul);
			solucion_emitir(soluciones, NOMBRE_ARCHIVOS, OFFSET_ARCHIVOS);
			lista_destruir(soluciones, destructor_solucion);
			resultado_destruir(resul);
		}else{
			buscador_busquedaPuntual(busq, (char*)lista_ver_primero(busquedas),INDICE, NOMBRE_ARCHIVOS, OFFSET_ARCHIVOS, POSICIONES_ARCHIVOS);
		}
		clock_t tiempo = clock() - tiempo_ini;

		float segsTot = (float) tiempo / CLOCKS_PER_SEC;

		printf("Tiempo de busqueda: %f segundos\n", segsTot);

		lista_destruir(busquedas,free);
		free(query);

}

bool validar_respuesta(int num){
	return ((num == 1) || (num ==2));
}

bool continuar_busqueda(){
	bool valida = false;
	int num;

	while(!valida){
		printf("Desea realizar una nueva busqueda? \n");
		printf("1: Si \n2: No \n");
		num = leer_numero();
		valida = validar_respuesta(num);
		if(!valida)
			printf("Su respuesta es invalida. Por favor ingrese 1 o 2 \n \n");
	}

	if (num == 1)
		return true;

	return false;
}

void buscar(char* directorio){
	printf("Cargando el lexico en memoria, espere un instante\n");
	size_t cant = obtenerCantidadDocs(INDICE);
	buscador_t* busq = buscador_crear(LEXICO,DIFERENTES, cant);
	printf("Cargado el lexico\n");

	bool continuar_buscando = true;

	while(continuar_buscando){
		realizar_busqueda(cant,busq);
		continuar_buscando = continuar_busqueda();
	}

	printf("\nAdios!! \n \n");
	buscador_destruir(busq);

}

void indexar(char* directorio){
	char** rutas;
	unsigned long cant;

	int aux = parserIndex_obtenerRutasDirectorios(directorio, &rutas, &cant);

	if (aux != PARSERINDEX_OK){
		printf ("Error al obtener los archivos\n");
		return;
	}

	aux = parserIndex_parsearArchivos(directorio, rutas,cant,SALIDA_PARSER, NOMBRE_ARCHIVOS, OFFSET_ARCHIVOS, POSICIONES_ARCHIVOS);

	if (aux == PARSERINDEX_OK){
		log_emitir("Paseo Realizado Correctamente", LOG_ENTRADA_INFORMATIVA_IMPORTANTE);
		aux = sorting_ordenarArchivo(SALIDA_PARSER, SALIDA_SORT);
		indexer_indexar(cant,SALIDA_SORT, INDICE, LEXICO, DIFERENTES, POSICIONES_ARCHIVOS);

		remove(SALIDA_PARSER);
		remove(SALIDA_SORT);
	}else{
		log_emitir("Parseo incorrecto", LOG_ENTRADA_ERROR);
	}

	for (unsigned long i = 0; i < cant; i++){
		free(rutas[i]);
	}
	free(rutas);
	log_emitir("Se termino de Indexar los documentos", LOG_ENTRADA_INFORMATIVA_IMPORTANTE);
}

void imprimir_ayuda(){
	fprintf(stdout,"OPCIONES \n");
	fprintf(stdout,"-h --Imprime en pantalla informacion de Ayuda. \n");
	fprintf(stdout,"-V --Imprime la versión del programa. \n");
	fprintf(stdout,"-i --Indexa un repositorio. Es la opcion por defecto. \n");
	fprintf(stdout,"-b --Busca una frase en un repositorio. \n");
	fprintf(stdout,"-r --Ruta del repositorio. \n");
}

//imprime la version del programa al stdout
void imprimir_version(){
	fprintf(stdout,"Trabajo Practico - Organizacion de Datos \n");
	fprintf(stdout,"Indexador - Buscador \n");
	fprintf(stdout,"Primer Cuatrimestre - 2013 \n");
	fprintf(stdout,"Grupo: 5 \n");
	fprintf(stdout," BUCHWALD, Martin Ezequiel (93155) \n GENENDER PEÑA, Ezequiel David (93163) \n WOITES, Jennifer Andrea (93274) \n");
}

int obtener_parametros(int argc,char* argv[],char** ruta_directorio){

	int opcion = OPC_INDEXAR;//opcion default

	*ruta_directorio = RUTA_DEFAULT; //directorio default

	//struct de lineas de comando
	struct option opciones[]={
		{"help",no_argument,NULL,'h'},//pos0
		{"version",no_argument,NULL,'V'},//pos1
		{"indexar",no_argument,NULL,'i'},//pos2
		{"buscar",no_argument,NULL,'b'},//pos3
		{"ruta_directorio",required_argument,NULL,'r'},//pos4
		{0,0,0,0}
	};

	char caracter;

	//mientras haya opciones las lee y las procesa
	while ((caracter = (getopt_long(argc,argv,"hVibr:",opciones,NULL)))!=-1){
		switch(caracter){
			case 'h'://help
				return OPC_IMPRIMIR_AYUDA;
			case 'V'://Version
				return OPC_IMPRIMIR_VERSION;
			case 'i'://indexar
				opcion = OPC_INDEXAR;
				break;
			case 'b'://buscar
				opcion = OPC_BUSCAR;
				break;
			case 'r'://ruta directorio
				if (strcmp(optarg,"-")!=0) //si son distintos
					*ruta_directorio = optarg;
				break;
			case '?'://error
				return OPC_ERROR;
		}
	}
	return opcion;
}


int main (int argc, char** argv){

	int opcion; //guarda el modo del programa

	char* directorio = NULL;

	opcion = obtener_parametros(argc,argv,&directorio);

	switch (opcion){
		case OPC_INDEXAR:
			indexar(directorio);
			//destruirTrasbordo(directorio);
			break;
		case OPC_BUSCAR:
			buscar(directorio);
			//destruirTrasbordo(directorio);
			break;
		case OPC_IMPRIMIR_AYUDA:
			imprimir_ayuda();
			break;
		case OPC_IMPRIMIR_VERSION:
			imprimir_version();
			break;
		case OPC_ERROR:
			fprintf(stderr,"Parametros no validos. Intente nuevamente \n");
			break;
	}

	return 0;
}

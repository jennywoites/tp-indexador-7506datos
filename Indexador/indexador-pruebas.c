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
#include "Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "Carpetas Compartidas/Codigos/trasbordoCodigo.h"
#include "Carpetas Compartidas/Compresion/zippeador.h"

#include <time.h>
#include <bits/time.h>

#define OPC_INDEXAR 1
#define OPC_BUSCAR 2
#define OPC_ERROR -1
#define OPC_IMPRIMIR_AYUDA 3
#define OPC_IMPRIMIR_VERSION 4

#define RUTA_DEFAULT "EL_REPO"

const char* SALIDA_PARSER = "parser.jem";
const char* SALIDA_SORT = "sort.jem";

const char* TEMP[] = {"indiceTEMP.jem","lexicoTEMP.jem" ,"diferentesTEMP.jem", "archsTEMP.jem", "offarchsTEMP.jem", "cantPosArchsTEMP.jem"};
#define CANT_RUTAS_NECESARIAS 6
const char* DEFINITIVOS[] = {"indice.jem","lexico.jem" ,"diferentes.jem", "archs.jem", "offarchs.jem", "cantPosArchs.jem"};

#define RUTA_INDICE 0
#define RUTA_LEXICO 1
#define RUTA_DIFERENTES 2
#define RUTA_PATHS 3
#define RUTA_OFFSET_ARCHS 4
#define RUTA_POS_X_ARCH 5


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

char** definir_nombres_archivos(const char* repositorio, const char** rutas_agregados ){
	char** elv = malloc (sizeof(char*) * CANT_RUTAS_NECESARIAS);
	for (size_t i = 0; i < CANT_RUTAS_NECESARIAS; i++){
		elv[i] = __crear_ruta_repo(repositorio, rutas_agregados[i]);
	}
	return elv;
}

void realizar_busqueda(size_t cant, buscador_t* busq, char** salidas, bool permitirImperfectas){
	printf("Ingrese Consulta:\n");
	char* query = leer_texto();
	if (strlen(query) == 0){
		free(query);
		printf("Su Consulta no tiene terminos relevantes. \n");
		return;
	}

	clock_t tiempo_ini = clock();

	lista_t* busquedas = parserQuery_parsearConsulta(query);

	if (lista_largo(busquedas) > 1){
		resultado_t* resul = buscador_buscar(busq, busquedas,salidas[RUTA_INDICE], salidas[RUTA_POS_X_ARCH]);
		lista_t* soluciones = resultado_realizarIntersecciones(resul);
		size_t cant_soluciones = lista_largo(soluciones);
		solucion_emitir(soluciones, salidas[RUTA_PATHS], salidas[RUTA_OFFSET_ARCHS]);
		lista_destruir(soluciones, destructor_solucion);
		resultado_destruir(resul);
		if (cant_soluciones == 0 && permitirImperfectas){
			buscador_busquedaImperfecta(busq, busquedas, salidas[RUTA_INDICE], salidas[RUTA_PATHS], salidas[RUTA_OFFSET_ARCHS], salidas[RUTA_POS_X_ARCH]);
		}
	}else{
		buscador_busquedaPuntual(busq, (char*)lista_ver_primero(busquedas),salidas[RUTA_INDICE], salidas[RUTA_PATHS], salidas[RUTA_OFFSET_ARCHS], salidas[RUTA_POS_X_ARCH]);
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

void buscar(const char* directorio, const char* repositorio, bool permitirImperfectas){
	printf("Cargando el lexico en memoria, espere un instante\n");

	char** salidas_def = definir_nombres_archivos(repositorio, DEFINITIVOS);
	char** salidas_temp = definir_nombres_archivos(repositorio, TEMP);

	for (size_t i = 0; i < CANT_RUTAS_NECESARIAS; i++){
		zippeador_descomprimir(salidas_def[i], salidas_temp[i]);
		free(salidas_def[i]);
	}
	free(salidas_def);

	size_t cant = obtenerCantidadDocs(salidas_temp[RUTA_INDICE]);
	buscador_t* busq = buscador_crear(salidas_temp[RUTA_LEXICO],salidas_temp[RUTA_DIFERENTES], cant);
	printf("Cargado el lexico\n");

	bool continuar_buscando = true;

	while(continuar_buscando){
		realizar_busqueda(cant,busq,  salidas_temp, permitirImperfectas);
		continuar_buscando = continuar_busqueda();
	}

	printf("\nAdios!! \n \n");
	buscador_destruir(busq);

	for (size_t i = 0; i < CANT_RUTAS_NECESARIAS; i++){
		remove(salidas_temp[i]);
		free(salidas_temp[i]);
	}
	free(salidas_temp);
}

void indexar(const char* directorio,const char* repositorio){
	char** rutas;
	unsigned long cant;

	char **salidas = definir_nombres_archivos(repositorio, TEMP);

	int aux = parserIndex_obtenerRutasDirectorios(directorio, &rutas, &cant);

	if (aux != PARSERINDEX_OK){
		printf ("Error al obtener los archivos\n");
		return;
	}

	aux = parserIndex_parsearArchivos(directorio, rutas,cant,SALIDA_PARSER, salidas[RUTA_PATHS], salidas[RUTA_OFFSET_ARCHS], salidas[RUTA_POS_X_ARCH]);

	if (aux == PARSERINDEX_OK){
		log_emitir("Paseo Realizado Correctamente", LOG_ENTRADA_INFORMATIVA_IMPORTANTE);
		aux = sorting_ordenarArchivo(SALIDA_PARSER, SALIDA_SORT);
		indexer_indexar(cant,SALIDA_SORT, salidas[RUTA_INDICE], salidas[RUTA_LEXICO], salidas[RUTA_DIFERENTES], salidas[RUTA_POS_X_ARCH]);

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


	char** salidas_def = definir_nombres_archivos(repositorio, DEFINITIVOS);

	for (size_t i = 0; i < CANT_RUTAS_NECESARIAS; i++){
		zippeador_comprimir(salidas[i], salidas_def[i]);
		remove(salidas[i]);
		free(salidas[i]);
		free(salidas_def[i]);
	}
	free(salidas);
	free(salidas_def);
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

int obtener_parametros(int argc,char* argv[],char** ruta_directorio, char** repo, bool* imperfectas){

	int opcion = OPC_INDEXAR;//opcion default

	*ruta_directorio = RUTA_DEFAULT; //directorio default
	*imperfectas = false;

	//struct de lineas de comando
	struct option opciones[]={
		{"help",no_argument,NULL,'h'},//pos0
		{"version",no_argument,NULL,'v'},//pos1
		{"indexar",no_argument,NULL,'i'},//pos2
		{"buscar",no_argument,NULL,'b'},//pos3
		{"ruta_directorio",required_argument,NULL,'d'},//pos4
		{"repositorio",required_argument,NULL,'r'},//pos5
		{"imperfectas",no_argument,NULL,'p'},//pos6
		{0,0,0,0}
	};

	char caracter;

	//mientras haya opciones las lee y las procesa
	while ((caracter = (getopt_long(argc,argv,"hvibd:r:p",opciones,NULL)))!=-1){
		switch(caracter){
			case 'h'://help
				return OPC_IMPRIMIR_AYUDA;
			case 'v'://Version
				return OPC_IMPRIMIR_VERSION;
			case 'i'://indexar
				opcion = OPC_INDEXAR;
				break;
			case 'b'://buscar
				opcion = OPC_BUSCAR;
				break;
			case 'd'://ruta directorio
				if (strcmp(optarg,"-")!=0) //si son distintos
					*ruta_directorio = optarg;
				break;
			case 'r':
				if (strcmp(optarg,"-")!=0) //si son distintos
					*repo = optarg;
				break;
			case 'p':
				*imperfectas = true;
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
	char* repositorio = NULL;
	bool imperfectas = false;

	opcion = obtener_parametros(argc,argv,&directorio, &repositorio, &imperfectas);

	if (!repositorio || !directorio) return 1;

	switch (opcion){
		case OPC_INDEXAR:
			indexar(directorio, repositorio);
			//destruirTrasbordo(directorio);
			break;
		case OPC_BUSCAR:
			buscar(directorio, repositorio, imperfectas);
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

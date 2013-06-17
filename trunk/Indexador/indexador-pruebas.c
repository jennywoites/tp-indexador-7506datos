#include <stdio.h>
#include "Indexador/Parseo/parserIndex.h"
#include "Indexador/Ordenamiento/sorting.h"
#include "Indexador/Indexado/indexer.h"
#include "Carpetas Compartidas/Log/log.h"
#include "Carpetas Compartidas/interaccion.h"
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <dirent.h>
#include "Buscador/buscador.h"
#include "Buscador/resultado.h"
#include "Buscador/Parseo/parserQuery.h"
#include "Carpetas Compartidas/TDAs/lista.h"
#include "Carpetas Compartidas/Manejo de Archivos/termino.h"
#include "Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "Carpetas Compartidas/Codigos/trasbordoCodigo.h"
#include "Carpetas Compartidas/Compresion/zippeador.h"

#include <time.h>
#include <bits/time.h>

#define OPC_ERROR -1
#define OPC_INDEXAR 1
#define OPC_BUSCAR_POR_FRASES 2
#define OPC_BUSCAR_PALABRAS 3
#define OPC_IMPRIMIR_AYUDA 4
#define OPC_IMPRIMIR_VERSION 5
#define OPC_AYUDA_USUARIO 6

#define RUTA_DEFAULT "EL_REPO"
#define REPO_DEFAULT "Repo"

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

void realizar_busqueda(size_t cant, buscador_t* busq, char** salidas, bool permitirImperfectas, bool permitirMontecarlo, size_t opcion_busqueda){
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

		lista_t* soluciones = NULL;
		size_t cant_soluciones = 0;

		switch(opcion_busqueda){
		case OPC_BUSCAR_POR_FRASES:
			soluciones = resultado_realizarIntersecciones(resul);
			solucion_emitir(soluciones, salidas[RUTA_PATHS], salidas[RUTA_OFFSET_ARCHS]);
			cant_soluciones = lista_largo(soluciones);
			break;
		case OPC_BUSCAR_PALABRAS:
			soluciones = obtenerAparicionDocumentos(resul);
			if (!soluciones){
				cant_soluciones = 0;
				printf("No hay resultados para su busqueda \n");
			}else{
				cant_soluciones = lista_largo(soluciones);
				if (cant_soluciones==0)
					printf("No hay resultados para su busqueda \n");
				if (cant_soluciones>0)
					printf("Las palabras buscadas aparecen en los siguientes documentos: \n");
				while (!lista_esta_vacia(soluciones)){
					unsigned int doc = atoi ( lista_borrar_primero(soluciones) );
					char* nombre_doc = __obtenerNombreDoc(salidas[RUTA_PATHS], salidas[RUTA_OFFSET_ARCHS], doc);
					printf("Documento: %s \n", nombre_doc);
					free(nombre_doc);
				}
			}
			break;
		}
		lista_destruir(soluciones, destructor_solucion);
		resultado_destruir(resul);
		if (cant_soluciones == 0){
			if (permitirImperfectas)
				buscador_busquedaImperfecta(busq, busquedas, salidas[RUTA_INDICE], salidas[RUTA_PATHS], salidas[RUTA_OFFSET_ARCHS], salidas[RUTA_POS_X_ARCH]);
			else if (permitirMontecarlo)
				buscador_busquedaImperfectaMontecarlo(busq, busquedas, salidas[RUTA_INDICE], salidas[RUTA_PATHS], salidas[RUTA_OFFSET_ARCHS], salidas[RUTA_POS_X_ARCH]);
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

bool validar_respuesta(char caract){
	return ((caract == '1') || (caract == '2'));
}

bool continuar_busqueda(){
	bool valida = false;
	char caract;
	int i = 0;

	while(!valida){
		printf("Desea realizar una nueva busqueda? \n");
		printf("1: Si \n2: No \n");
		caract = getchar();

		valida = validar_respuesta(caract);
		if(!valida)
			printf("Su respuesta es invalida. Por favor ingrese 1 o 2 \n \n");
	}
	while (getchar() != '\n')
			i++;

	if (caract == '1')
		return true;


	return false;
}

void buscar(const char* directorio, const char* repositorio, bool permitirImperfectas,bool permitirMontecarlo ,size_t opcion_busqueda){
	char** salidas_def = definir_nombres_archivos(repositorio, DEFINITIVOS);
	bool correcto = true;
	for (size_t i = 0; i < CANT_RUTAS_NECESARIAS && correcto; i++){
		FILE* aux = fopen(salidas_def[i], "r");
		if (!aux) correcto = false;
		else fclose(aux);
	}

	if (!correcto){
		for (size_t i = 0; i < CANT_RUTAS_NECESARIAS; i++)
			free(salidas_def[i]);
		free(salidas_def);
		printf("No se puede realizar la busqueda pues no se encuentran todos los archivos necesarios\n");
		return;
	}

	printf("Cargando el lexico en memoria, espere un instante\n");

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
		realizar_busqueda(cant,busq,  salidas_temp, permitirImperfectas, permitirMontecarlo,opcion_busqueda);
		continuar_buscando = continuar_busqueda();
	}

	printf("\nAdios!! \n \n");
	buscador_destruir(busq);

	for (size_t i = 0; i < CANT_RUTAS_NECESARIAS; i++){
		remove(salidas_temp[i]);
		free(salidas_temp[i]);
	}
	free(salidas_temp);
	destruccionCache();
}

int filtro_aux (struct dirent* d){
	if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) return 0;

	//Por ahora, a los sub-directorios los descartamos (y cualquier cosa que no sea un archivo)
	return 1;
}



bool existeDirectorio(const char* directorio){
	struct dirent** directorios = NULL;
	int valor;
	valor = scandir(directorio, &directorios, filtro_aux, NULL);
	for (int i = 0; i < valor; i++)
		free(directorios[i]);
	free(directorios);
	return valor != -1;
}

void indexar(const char* directorio,const char* repositorio){
	char** rutas;
	unsigned long cant;

	if (!existeDirectorio(directorio)){
		printf("No se puede indexar dado que no existe el directorio indicado\n");
		return;
	}

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
	fprintf(stdout,"-h --help              Imprime en pantalla informacion de Ayuda. \n");
	fprintf(stdout,"-u --user              Explicacion detallada del funcionamiento para el usuario. \n");
	fprintf(stdout,"-v --version           Imprime la versión del programa. \n");
	fprintf(stdout,"-i --indexar           Indexa un repositorio. Es la opcion por defecto. \n");
	fprintf(stdout,"-b --buscar            Busca una frase en un repositorio. \n");
	fprintf(stdout,"-t --buscar_todas      Busca varias palabras en un repositorio. \n");
	fprintf(stdout,"-p --imperfectas       Busca una frase, permite una busqueda aproximada en caso de no encontrar solucion exacta. \n");
	fprintf(stdout,"-m --montecarlo        Busca una frase, permite una busqueda aproximada por metodo de Montecarlo. \n");
	fprintf(stdout,"-d --ruta_directorio   Ruta del repositorio. \n");
	fprintf(stdout,"-r --repositorio       Nombre del repositorio. \n");
}

void imprimir_ayuda_usuario(){
	fprintf(stdout, "\n Bienvenido al Buscador - Indexador! \n \n");
	fprintf(stdout, "Para poder utilizar el programa, primero debe indexar su repositorio. \nPara ello, elija la ruta del repositorio a indexar y el nombre que desee utilizar.\nPor ejemplo, la ruta 'usuario/repositorio' y el nombre del repositorio 'MiRepositorio' se llamaran con los siguientes comandos: \n-i -r usuario/repositorio -d MiRepositorio \n");
	fprintf(stdout, "El repositorio comenzara a indexarse. Esta operacion puede demorar algunos minutos, dependiendo del tamaño del repositorio a indexar. \n");
	fprintf(stdout, "Ahora que tenemos nuestro repositorio, podemos realizar busquedas con los distintos comandos. \nLas busquedas pueden ser tanto por palabras como por frases. \n");
	fprintf(stdout, "Si queremos realizar una busqueda por palabra o frase exacta debemos llamar al programa con los siguientes comandos: \n-b -r MiRepositorio \n");
	fprintf(stdout, "Si nos interesa buscar palabras sueltas en un texto, principalmente para encontrar textos que sean de algun genero, debemos llamar al programa con los siguientes comandos: \n-t -r Mi repositorio \n");
	fprintf(stdout, "Si queremos hacer busquedas por frases, pero en caso de no encontrarlas, permitir que el programa elimine algunas palabras, utilizamos el metodo de Montecarlo o el metodo que permite Imperfectas \nPara ello, activamos el -m para Montecarlo o -p para imperfectas ademas de los comandos necesarios para busqueda tradicional o para busqueda por palabras.\n");
	fprintf(stdout, "Si en cualquier momento olvidas los comandos de inicializacion puedes escribir -h o --help y se desplegara la ayuda: \n \n");
	imprimir_ayuda();
	fprintf(stdout, "\n Esperamos que disfrutes el programa!! \n \n");
}

//imprime la version del programa al stdout
void imprimir_version(){
	fprintf(stdout,"Trabajo Practico - Organizacion de Datos \n");
	fprintf(stdout,"Indexador - Buscador \n");
	fprintf(stdout,"Primer Cuatrimestre - 2013 \n");
	fprintf(stdout,"Grupo: 5 \n");
	fprintf(stdout," BUCHWALD, Martin Ezequiel (93155) \n GENENDER PEÑA, Ezequiel David (93163) \n WOITES, Jennifer Andrea (93274) \n");
}

int obtener_parametros(int argc,char* argv[],char** ruta_directorio, char** repo, bool* imperfectas, bool* montecarlo){

	int opcion = OPC_INDEXAR;//opcion default

	*ruta_directorio = RUTA_DEFAULT; //directorio default
	*repo = REPO_DEFAULT; //repositorio default
	*imperfectas = false;
	*montecarlo = false;

	//struct de lineas de comando
	struct option opciones[]={
		{"help",no_argument,NULL,'h'},//pos0
		{"user",no_argument,NULL,'u'},//pos1
		{"version",no_argument,NULL,'v'},//pos1
		{"indexar",no_argument,NULL,'i'},//pos2
		{"buscar",no_argument,NULL,'b'},//pos3
		{"buscar_todas",no_argument,NULL,'t'},//pos3
		{"ruta_directorio",required_argument,NULL,'d'},//pos4
		{"repositorio",required_argument,NULL,'r'},//pos5
		{"imperfectas",no_argument,NULL,'p'},//pos6
		{"montecarlo",no_argument,NULL,'m'},//pos6
		{0,0,0,0}
	};

	char caracter;

	//mientras haya opciones las lee y las procesa
	while ((caracter = (getopt_long(argc,argv,"huvibtd:r:pm",opciones,NULL)))!=-1){
		switch(caracter){
			case 'h'://help
				return OPC_IMPRIMIR_AYUDA;
			case 'u'://Ayuda para usuarios
				return OPC_AYUDA_USUARIO;
			case 'v'://Version
				return OPC_IMPRIMIR_VERSION;
			case 'i'://indexar
				opcion = OPC_INDEXAR;
				break;
			case 'b'://buscar
				opcion = OPC_BUSCAR_POR_FRASES;
				break;
			case 't'://buscar todas las palabras
				opcion = OPC_BUSCAR_PALABRAS;
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
			case 'm':
				*montecarlo = true;
				break;
			case '?'://error
				return OPC_ERROR;
		}
	}
	return opcion;
}


int main (int argc, char** argv){
	srand(time(NULL));
	int opcion; //guarda el modo del programa

	char* directorio = NULL;
	char* repositorio = NULL;
	bool imperfectas = false;
	bool montecarlo = false;

	opcion = obtener_parametros(argc,argv,&directorio, &repositorio, &imperfectas, &montecarlo);

	if (!repositorio || !directorio) return 1;

	switch (opcion){
		case OPC_INDEXAR:
			indexar(directorio, repositorio);
			//destruirTrasbordo(directorio);
			break;
		case OPC_BUSCAR_POR_FRASES:
			buscar(directorio, repositorio,imperfectas,montecarlo,OPC_BUSCAR_POR_FRASES);
			break;
		case OPC_BUSCAR_PALABRAS:
			buscar(directorio, repositorio,imperfectas,montecarlo,OPC_BUSCAR_PALABRAS);
			break;
		case OPC_IMPRIMIR_AYUDA:
			imprimir_ayuda();
			break;
		case OPC_IMPRIMIR_VERSION:
			imprimir_version();
			break;
		case OPC_AYUDA_USUARIO:
			imprimir_ayuda_usuario();
			break;
		case OPC_ERROR:
			fprintf(stderr,"Parametros no validos. Intente nuevamente \n");
			break;
	}

	return 0;
}

#include "resultado.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../Carpetas Compartidas/TDAs/abb.h"
#include "../Carpetas Compartidas/TDAs/heap.h"
#include "../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include <string.h>

/**************************************************************************************/
/*                                     SOLUCION                                       */
/**************************************************************************************/

// Por ahora son resultados triviales, cuando se quieran mejorar, se toquetea por aca
typedef struct {
	abb_t* arbol_resul;
	size_t doc;
	bool visitado;
}solucion_t;

//Funcion para comparar pesos de los arboles
int comparador_stringPos (const char* a, const char* b){
	size_t pos1 = atoi(a);
	size_t pos2 = atoi(b);
	return pos1 - pos2;
}

solucion_t* solucion_crear(size_t num){
	solucion_t* sol = malloc (sizeof(solucion_t));
	if (!sol) return NULL;
	sol->doc = num;
	sol->arbol_resul = abb_crear(comparador_stringPos,NULL);
	sol->visitado = true;
	return sol;
}

void solucion_destruir(solucion_t* solucion){
	if (!solucion) return;
	abb_destruir(solucion->arbol_resul);
	free(solucion);
}

int __obtenerNumTermino(lista_t* terminos, termino_t* term_actual, int inicio){
	char* buscado = termino_obtenerPalabra(term_actual);
	lista_iter_t* iter = lista_iter_crear(terminos);
	int num = -1;
	bool encontrado = false;
	for (int i = 0; i < inicio; i++){
		lista_iter_avanzar(iter);
		num++;
	}
	while (!lista_iter_al_final(iter) && !encontrado){
		char* comp = lista_iter_ver_actual(iter);
		encontrado = (strcmp(comp, buscado) == 0);
		num++;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	free(buscado);
	if (!encontrado)
		return -1;
	return num;
}

int __crearListasPosiciones(solucion_t* solucion, lista_t** posiciones, lista_t* terminos){
	abb_iter_t* iter = abb_iter_in_crear(solucion->arbol_resul);
	while (!abb_iter_in_al_final(iter)){
		const char* strPos = abb_iter_in_ver_actual(iter);
		size_t numPos = atoi(strPos);
		termino_t* term_actual = abb_obtener(solucion->arbol_resul,strPos);
		int numTer = __obtenerNumTermino(terminos, term_actual, 0);
		if (numTer == -1){//? Por ahora devolvemos null o algo asi...
			abb_iter_in_destruir(iter);
			for (size_t i = 0; i < lista_largo(terminos); i++) lista_destruir(posiciones[i],free);
			free(posiciones);
			return -1;
		}
		//FIXME ??
		do {
//			printf("apreta enter %d\n", numTer); getchar();
			size_t numPosFin = 0;
//			if (numPos >= numTer){
//				numPosFin = numPos - numTer;
//			}else{
//				abb_iter_in_avanzar(iter);
//				break;
//			}
			if (numPos < numTer)
				break;
			numPosFin = numPos - numTer;
			size_t* punteroPos = malloc (sizeof(size_t));
			*punteroPos = numPosFin;
//			if (numPos < 1000)
//				printf("Pos : %lu %d\n", numPosFin, numTer);
			lista_insertar_ultimo(posiciones[numTer],punteroPos);
			numTer = __obtenerNumTermino(terminos, term_actual, numTer+1);
		}while (numTer != -1);

		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
	return 0;
}

void __copiarListas(lista_t* dst, lista_t* org){
lista_iter_t* iter = lista_iter_crear(org);
	while (!lista_iter_al_final(iter)){
		size_t* act = lista_iter_ver_actual(iter);
		size_t* cpy = malloc (sizeof(size_t));
		*cpy = *act;
		lista_insertar_ultimo(dst, cpy);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
}

lista_t* __realizarIntersecciones(lista_t** posiciones, size_t cant){
	lista_t* interseccion = lista_crear();
	__copiarListas(interseccion, posiciones[0]);

	for (size_t i = 1; i < cant; i++ ){
		if (lista_largo(interseccion) == 0) return interseccion;
		lista_iter_t* iterInt = lista_iter_crear(interseccion);
		//lista_iter_t* iterSec = lista_iter_crear(posiciones[i]);
		lista_t* aux = lista_crear();
		while (!lista_iter_al_final(iterInt)){
			lista_iter_t* iterSec = lista_iter_crear(posiciones[i]);
			size_t* pos = lista_iter_ver_actual(iterInt);
			bool pasado = false;
			while(!lista_iter_al_final(iterSec) && !pasado){
				size_t* posSec = lista_iter_ver_actual(iterSec);
				pasado = (*posSec) > (*pos);
				//printf("INTERSECO %lu con %lu \n", *pos, *posSec);
				if (*posSec == *pos){
					size_t* cpy = malloc (sizeof(size_t));
					*cpy = *pos;
					lista_insertar_ultimo(aux, cpy);
				}
				lista_iter_avanzar(iterSec);
			}
			lista_iter_destruir(iterSec);
			lista_iter_avanzar(iterInt);
		}
		lista_iter_destruir(iterInt);
		//lista_iter_destruir(iterSec);
		lista_destruir(interseccion, free);
		interseccion = aux;
	}
	return interseccion;
}

int comparacionListas(const void* a, const void* b){
	lista_t* l1 = (lista_t*)a;
	lista_t* l2 = (lista_t*)b;
	return (lista_largo(l1) - lista_largo(l2));
}

lista_t* solucion_determinarCorrecto(solucion_t* solucion, lista_t* terminos){
	if (!solucion || !terminos) return NULL;
	lista_t** posiciones = malloc (sizeof(lista_t*) * lista_largo(terminos));
	for (size_t i = 0; i < lista_largo(terminos); i++){
		lista_t* list = lista_crear();
		//lista_insertar_ultimo(posiciones[i], list);
		posiciones[i] = list;
	}

	if (__crearListasPosiciones(solucion, posiciones, terminos) == -1){
		return NULL;
	}

	heapsort((void*)posiciones,lista_largo(terminos),  comparacionListas );

	lista_t* interseccion = __realizarIntersecciones(posiciones, lista_largo(terminos));
	for (size_t i = 0; i < lista_largo(terminos);i++){
		lista_destruir(posiciones[i], free);
	}
	free(posiciones);
	return interseccion;
}

/**************************************************************************************/
/*                                     RESULTADO                                      */
/**************************************************************************************/

struct resultado{
	lista_t* soluciones;
};

void destructor_soluciones(void* elem){
	solucion_destruir((solucion_t*)elem);
}

void resultado_destruir(resultado_t* resultado){
	if (!resultado) return;
	lista_destruir(resultado->soluciones, destructor_soluciones);
	free(resultado);
}

void agregarTodasLasSoluciones(resultado_t* resul, termino_t* actual, lista_t* infoTermino);
void agregarYeliminarSoluciones(resultado_t* resul, termino_t* termino, lista_t* infoTermino);
void indicarSolucionesComoNoVisitadas(resultado_t* resul);
void sacarSolucionesNoVisitadas(resultado_t* resul);

resultado_t* resultado_crear(termino_t** terminos, size_t cant, const char* dirOffsets){
	heapsort((void*)terminos, cant, terminos_comparar);

	//Por ahora descarto si un cierto termino no existe en los registros
	if (!terminos[0]) return NULL;
	resultado_t* resul = malloc (sizeof(resultado_t));
	resul->soluciones = lista_crear();

	bool primero = true;
	for (size_t i = 0; i < cant; i++){
		termino_t* actual = terminos[i];
		//la lista que devuelve es: pos 0, documentos (num) en los que se encuentra el
		//							termino. Posiciones siguientes (tantas como la cant
		//							de elementos de la primera lista) son los numeros de posiciones.
		//termino_obtenerInformacion(actual, dirOffsets);
		lista_t* infoTermino = termino_decodificarPunteros(actual, dirOffsets);
		if (!infoTermino){
			resultado_destruir(resul);
			return NULL;
		}

		if (primero){
			agregarTodasLasSoluciones(resul, actual, infoTermino);
			primero = false;
		}else{
			agregarYeliminarSoluciones(resul, actual, infoTermino);
		}
		indicarSolucionesComoNoVisitadas(resul);

	}
	return resul;
}

char* __sizeToString(size_t num){
	size_t cant = 0;
	size_t aux = num;
	while (aux > 0){
		aux /= 10;
		cant++;
	}
	char* str = malloc (sizeof(char) * (cant+1));
	for (unsigned int i = 1; i <= cant; i++){
		str[cant - i ] = '0' + num % 10;
		num /= 10;
	}
	str[cant] = '\0';
	return str;
}


void agregarTodasLasSoluciones(resultado_t* resul, termino_t* termino, lista_t* infoTermino){
	if (!resul) return;
	if (!termino) return;
	if (!infoTermino) return;

	lista_t* documentos = lista_borrar_primero(infoTermino);
	lista_iter_t* iter = lista_iter_crear(documentos);

	while (!lista_iter_al_final(iter)){
		size_t doc = *((size_t*) (lista_iter_ver_actual(iter)));
		solucion_t* sol = solucion_crear(doc);
		lista_t* posiciones = lista_borrar_primero(infoTermino);
		lista_iter_t* iterPos = lista_iter_crear(posiciones);
		while (!lista_iter_al_final(iterPos)){
			size_t numPos = *((size_t*)(lista_iter_ver_actual(iterPos)));
			char* clave = __sizeToString(numPos);
			abb_guardar(sol->arbol_resul,clave, termino);
			free(clave);
			lista_iter_avanzar(iterPos);
		}
		lista_insertar_ultimo(resul->soluciones, sol);
		lista_iter_destruir(iterPos);
		lista_destruir(posiciones, free);
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(documentos, free);
	lista_destruir(infoTermino, NULL);
}

solucion_t* documento_en_resolucion(resultado_t* resul, size_t num_doc){
	if (!resul)
		return NULL;
	lista_iter_t* iter = lista_iter_crear(resul->soluciones);
	solucion_t* sol = NULL;
	while (!lista_iter_al_final(iter) && !sol){
		solucion_t* aux = lista_iter_ver_actual(iter);
		if  (aux->doc == num_doc)
			sol = aux;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return sol;
}

void agregarYeliminarSoluciones(resultado_t* resul, termino_t* termino, lista_t* infoTermino){
	if (!resul) return;
	if (!termino) return;
	if (!infoTermino) return;

	lista_t* documentos = lista_borrar_primero(infoTermino);
	lista_iter_t* iter = lista_iter_crear(documentos);
	while (!lista_iter_al_final(iter)){
		lista_t* posiciones = lista_borrar_primero(infoTermino);
		size_t num_doc = *((size_t*)(lista_iter_ver_actual(iter)));
		solucion_t* sol = documento_en_resolucion(resul, num_doc);
		if (!sol){
			lista_destruir(posiciones, free);
			lista_iter_avanzar(iter);
			continue;
		}
		sol->visitado = true;
		lista_iter_t* iterPos = lista_iter_crear(posiciones);
		while (!lista_iter_al_final(iterPos)){
			size_t numPos = *((size_t*)(lista_iter_ver_actual(iterPos)));
			char* clave = __sizeToString(numPos);
			abb_guardar(sol->arbol_resul,clave, termino);
			free(clave);
			lista_iter_avanzar(iterPos);
		}

		lista_iter_destruir(iterPos);
		lista_destruir(posiciones, free);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	lista_destruir(documentos, free);
	lista_destruir(infoTermino, NULL);

	sacarSolucionesNoVisitadas(resul);
}

void indicarSolucionesComoNoVisitadas(resultado_t* resul){
	if (!resul) return;
	lista_iter_t* iter = lista_iter_crear(resul->soluciones);
	while (!lista_iter_al_final(iter)){
		solucion_t* solucion = lista_iter_ver_actual(iter);
		solucion->visitado = false;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
}

void sacarSolucionesNoVisitadas(resultado_t* resul){
	if (!resul) return;
	lista_iter_t* iter = lista_iter_crear(resul->soluciones);

	while(!lista_iter_al_final(iter)){
		solucion_t* solucion = lista_iter_ver_actual(iter);
		if (!solucion->visitado){
			lista_borrar(resul->soluciones, iter);
			solucion_destruir(solucion);
		}else{
			lista_iter_avanzar(iter);
		}
	}
	lista_iter_destruir(iter);
}

void resultado_emitirListado(resultado_t* resultado, lista_t* query, const char* paths, const char* offsets){
	if (!resultado || !query) return;
	bool hay_solucion = false;
	lista_iter_t* iter_soluciones_posibles = lista_iter_crear(resultado->soluciones);
	while (!lista_iter_al_final(iter_soluciones_posibles)){
		solucion_t* actual = lista_iter_ver_actual(iter_soluciones_posibles);
		lista_t* lista_de_solucion = solucion_determinarCorrecto(actual, query);
		if (lista_largo(lista_de_solucion) > 0){
			hay_solucion = true;

			char* nombre_doc = __obtenerNombreDoc(paths, offsets,actual->doc);
			printf("El documento %s es solucion a la busqueda, en las siguientes posiciones:\n", nombre_doc);
			free(nombre_doc);

			lista_iter_t* iter_sol = lista_iter_crear(lista_de_solucion);
			while (!lista_iter_al_final(iter_sol)){
				size_t* sol = lista_iter_ver_actual(iter_sol);
				printf("Posicion: %u\n", *sol);
				lista_iter_avanzar(iter_sol);
			}
			lista_iter_destruir(iter_sol);
		}
		lista_destruir(lista_de_solucion, free);
		lista_iter_avanzar(iter_soluciones_posibles);
	}
	lista_iter_destruir(iter_soluciones_posibles);
	if (!hay_solucion)
		printf("No hay solucion a la consulta\n");
}

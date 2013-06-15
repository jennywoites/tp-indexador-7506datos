#include "termino.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/Codigos/trasbordoCodigo.h"
#include <sys/stat.h>

size_t CANT_DOCS = 0;

struct termino{
	char* termino;
	size_t offset;
	size_t frecuencia;
};

termino_t* termino_crear (const char* term, size_t offset, size_t frec){
	if (!term) return NULL;

	termino_t* t = malloc (sizeof(termino_t));
	if (!t) return NULL;

	t->termino = malloc (sizeof(char) * (strlen(term)+1));
	if (!t->termino){
		free(t);
		return NULL;
	}

	strcpy(t->termino, term);
	t->offset = offset;
	t->frecuencia = frec;
	return t;
}

void termino_destruir(termino_t* termino){
	if (!termino) return;

	free(termino->termino);
	free(termino);
}

char* termino_obtenerPalabra(termino_t* term){
	if (!term) return NULL;

	char* copy = malloc (sizeof(char) * (strlen(term->termino) + 1));
	if (!copy) return NULL;
	strcpy(copy, term->termino);
	return copy;
}

termino_t* termino_leer(termino_t* termino_anterior, debuffer_t* debuff_FrontCoding, FILE* archDiferentes){
/*	char* linea = obtenerLinea(archFrontCoding);
	char* repetidos = strtok(linea, ";");
	char* distintos = strtok(NULL, ";");
	char* offset = strtok(NULL, ";");
	char* frecuencia = strtok(NULL, ";");

	if (repetidos == NULL || distintos == NULL || offset == NULL || frecuencia == NULL){
		free(linea);
		return NULL;
	}

	size_t rep = atoi(repetidos);
	size_t dist = atoi(distintos);
	size_t off = atoi(offset);
	size_t frec = atoi (frecuencia);

	free(linea);
*/
	/*size_t rep = decodificador_decodificarGamma(debuff_FrontCoding);
	size_t dist = decodificador_decodificarGamma(debuff_FrontCoding);
	size_t off = decodificador_decodificarDelta(debuff_FrontCoding);
	size_t frec = decodificador_decodificarDelta(debuff_FrontCoding);*/
	size_t rep = descomprimir_LexicoRepetidos(debuff_FrontCoding);
	size_t dist = descomprimir_LexicoDiferentes(debuff_FrontCoding);
	size_t off = descomprimir_LexicoOffset(debuff_FrontCoding);
	size_t frec = descomprimir_FrecuenciaDocumentos(debuff_FrontCoding);

	if( rep == NO_NUMERO || dist == NO_NUMERO || off == NO_NUMERO || frec == NO_NUMERO )
		return NULL;
	rep--;
	//off--;
	
	
	char* cad = malloc (sizeof(char) * (rep + dist + 1));
	size_t i;
	for (i = 0; i < rep && termino_anterior; i++)
		cad[i] = termino_anterior->termino[i];

	for (i = 0; i < dist; i++)
		cad[i + rep] = fgetc(archDiferentes);
	cad[rep + dist] = '\0';

	size_t off_ant = 0;
	if (termino_anterior)
		off_ant = termino_anterior->offset;

	termino_t* t = termino_crear(cad, off + off_ant, frec);
	free(cad);

	return t;

}

int terminos_comparar(const void* a,const void* b){
	termino_t* t1 = (termino_t*) a;
	termino_t* t2 = (termino_t*) b;
	if (!t1) return -1;
	if (!t2) return +1;
	return t1->frecuencia - t2->frecuencia;
}

void termino_imprimir(termino_t* termino){
	if (!termino){
		printf("Termino NULL\n");
		return;
	}

	printf("El termino %s tiene frecuencia %u\n", termino->termino, termino->frecuencia);
}

lista_t* obtener_listado(debuffer_t* debuffer, size_t cant_documentos, const char* ruta_tams);

//Si no logro abrir el archivo o la frecuencia del termino es 0, devuelve NULL
lista_t* termino_decodificarPunteros(termino_t* termino,const char* ruta, const char* ruta_tams){
	if (!termino) return NULL;
	FILE* arch = fopen(ruta, lectura_archivos());

	if (!arch)
		return NULL;

	size_t offset = termino->offset;
	size_t frecuencia = termino->frecuencia;

	if (frecuencia == 0){
		fclose(arch);
		return NULL;
	}

	//Calculo la cantidad de bytes de offset
	size_t offset_bytes = offset / 8;
	size_t bits_a_desechar = offset % 8;

	struct stat st;
	stat(ruta, &st);
	size_t cant_bytes = st.st_size;
	debuffer_t* debuffer = debuffer_crear(arch, cant_bytes - offset_bytes);


	//Posicionarse en el archivo a un lugar del debuffer
	fseek(arch, offset_bytes,SEEK_SET); //SEEK_SET offset desde el inicio del archivo
	debuffer_descartar_bits(debuffer,bits_a_desechar);

	lista_t* listado = obtener_listado(debuffer, frecuencia, ruta_tams);
	fclose(arch);
	debuffer_destruir(debuffer);
	return listado;
}

lista_t* obtener_listado(debuffer_t* debuffer, size_t cant_documentos, const char* ruta_tams){
	if(!debuffer)
		return NULL;

	float p = (float) cant_documentos / CANT_DOCS;

	lista_t* listado_datos = lista_crear();
	lista_t* documentos = lista_crear();

	size_t frec_usada = cant_documentos;
	bool complementar = false;
	if ( p > 0.5){
		frec_usada = CANT_DOCS - cant_documentos;
		complementar = true;
		p = 1 - p;
	}

	float p_prima = p;
	size_t frec_prima = frec_usada;
	size_t n_prima = CANT_DOCS;

	size_t num_doc_actual = 0;
	for (size_t i = 0; i < frec_usada;i++){
		size_t b = calcular_B_optimo(p_prima);

		size_t distancia = descomprimir_IndiceDistanciaDocumentos(debuffer, b);
		num_doc_actual += distancia;
		size_t* actual = malloc (sizeof(size_t));
		*actual = num_doc_actual;
		lista_insertar_ultimo(documentos,actual );

		frec_prima--;
		n_prima -= distancia;
		if (n_prima > 0) p_prima = (float) frec_prima / n_prima;
		if (p_prima == 1){
			for (size_t j = num_doc_actual + 1;j <= CANT_DOCS;j++){
				actual = malloc (sizeof(size_t));
				*actual = j;
				lista_insertar_ultimo(documentos, actual);
			}
			break;
		}
	}

	if (complementar){
		lista_t* cmpt = complementarLista(documentos, CANT_DOCS);
		lista_destruir(documentos, free);
		documentos = cmpt;
	}
	lista_insertar_primero(listado_datos, documentos);

	size_t documentos_levantados[lista_largo(documentos)];
	size_t i = 0;
	lista_iter_t* iter = lista_iter_crear(documentos);
	while (!lista_iter_al_final(iter)){
		size_t* doc = lista_iter_ver_actual(iter);
		documentos_levantados[i] = *doc;
		i++;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);


	for (i = 0; i < cant_documentos; i++){
		size_t posicion_actual = 0;
		lista_t* posiciones = lista_crear();
		size_t cant_posiciones = descomprimir_FrecuenciaPosiciones(debuffer);
		size_t total_pos = obtenerCantDocumentos(ruta_tams , documentos_levantados[i]);
		float p_pos = (float) cant_posiciones / total_pos;
//		printf("posiciones %lu de %lu, p = %f\n", cant_posiciones, total_pos, p_pos);
//		getchar();

		size_t n_pos_prima = total_pos;
		size_t frec_pos_prima = cant_posiciones;

		for (size_t j = 0; j < cant_posiciones; j++){
			size_t b_pos = calcular_B_optimo(p_pos);
			size_t distancia = descomprimir_IndiceDistanciaPosiciones(debuffer, b_pos);
			posicion_actual += distancia;
			size_t* valor = malloc (sizeof(size_t));
			*valor = posicion_actual;
			lista_insertar_ultimo(posiciones, valor);

			n_pos_prima -= distancia;
			frec_pos_prima--;
			if (n_pos_prima > 0) p_pos = (float) frec_pos_prima / n_pos_prima;

			if (p_pos == 1){
				for (size_t k = posicion_actual + 1; k <= total_pos; k++){
					size_t* valor = malloc (sizeof(size_t));
					*valor = k;
					lista_insertar_ultimo(posiciones, valor);
				}
				break;
			}
		}
		lista_insertar_ultimo(listado_datos, posiciones);
	}

	/*if(!listado_datos)
		return NULL;

	size_t cant_doc_actual = 0;
	unsigned int cant_posiciones;
	unsigned int pos_actual;

	unsigned int numero;
	unsigned int numero_anterior;
	size_t* nueva_pos;

	while(cant_doc_actual < cant_documentos){
		//num_doc_actual += decodificador_decodificarGamma(debuffer);
		num_doc_actual += descomprimir_IndiceDistanciaDocumentos(debuffer, b);
		lista_t* listado_documentos = lista_ver_primero(listado_datos);
		size_t* actual = malloc (sizeof(size_t));
		*actual = num_doc_actual;
		lista_insertar_ultimo(listado_documentos,actual );
		//cant_posiciones = decodificador_decodificarDelta(debuffer);
		cant_posiciones = descomprimir_FrecuenciaPosiciones(debuffer);
		pos_actual = 0;
		numero_anterior = 0;
		lista_t* listado_posiciones = lista_crear();
		lista_insertar_ultimo(listado_datos, listado_posiciones);
		while(pos_actual < cant_posiciones){
			//numero = decodificador_decodificarDelta(debuffer);
			numero = descomprimir_IndiceDistanciaPosiciones(debuffer);
			nueva_pos = malloc(sizeof(size_t));
			if (!nueva_pos){
				lista_destruir(listado_datos, NULL); //despues hay que ponerle el destructor
				return NULL;
			}
			*nueva_pos = numero + numero_anterior;
			numero_anterior = *nueva_pos;
			lista_insertar_ultimo(listado_posiciones, nueva_pos);
			pos_actual ++;
		}
		cant_doc_actual ++;
	}
*/
	return listado_datos;
}


void termino_setearCantDocs(size_t valor){
	CANT_DOCS = valor;
}

float calculoImportancia (lista_t* modif, lista_t* org){
	size_t suma_total = 0;
	size_t modularizador = lista_largo(org) - 1;

	lista_iter_t* iter = lista_iter_crear(org);

	while (!lista_iter_al_final(iter)){
		termino_t* termino = lista_iter_ver_actual(iter);
		if (termino)
			suma_total += termino->frecuencia;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);

	size_t sumaMod = 0;

	lista_iter_t* iterMod = lista_iter_crear(modif);
	while (!lista_iter_al_final(iterMod)){
		termino_t* termino = lista_iter_ver_actual(iterMod);
		if (termino){
			sumaMod += (suma_total - termino->frecuencia);
		}
		lista_iter_avanzar(iterMod);
	}
	lista_iter_destruir(iterMod);
	float imp = (float) sumaMod / suma_total;
	imp /= modularizador;
	return imp;
}

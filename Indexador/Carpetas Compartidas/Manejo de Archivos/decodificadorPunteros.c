#include "decodificadorPunteros.h"
#include "../Codigos/decodificador.h"
#include "debuffer.h"

#include <sys/stat.h>
/*
lista_t* obtener_listado(debuffer_t* debuffer, size_t cant_documentos);

//Si no logro abrir el archivo o la frecuencia del termino es 0, devuelve NULL
lista_t* decodificar_punteros(const char* ruta, termino_t* termino){

	FILE* arch = fopen(ruta, "r");

	if (!arch)
		return NULL;

	size_t offset = termino_obtener_offset(termino);
	size_t frecuencia = termino_obtener_frecuencia(termino);

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

	lista_t* listado = obtener_listado(debuffer, frecuencia);
	fclose(arch);
	debuffer_destruir(debuffer);
	return listado;
}

lista_t* obtener_listado(debuffer_t* debuffer, size_t cant_documentos){
	if(!debuffer)
		return NULL;

	lista_t* listado_datos = lista_crear();
	lista_insertar_primero(listado_datos, lista_crear());

	if(!listado_datos)
		return NULL;

	size_t cant_doc_actual = 0;
	unsigned int cant_posiciones;
	unsigned int pos_actual;
	size_t num_doc_actual = 0;
	unsigned int numero;
	unsigned int numero_anterior;
	size_t* nueva_pos;

	while(cant_doc_actual < cant_documentos){
		num_doc_actual += decodificador_decodificarGamma(debuffer);
		lista_t* listado_documentos = lista_ver_primero(listado_datos);
		size_t* actual = malloc (sizeof(size_t));
		*actual = num_doc_actual;
		lista_insertar_ultimo(listado_documentos,actual );
		cant_posiciones = decodificador_decodificarDelta(debuffer);
		pos_actual = 0;
		numero_anterior = 0;
		lista_t* listado_posiciones = lista_crear();
		lista_insertar_ultimo(listado_datos, listado_posiciones);
		while(pos_actual < cant_posiciones){
			numero = decodificador_decodificarDelta(debuffer);
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

	return listado_datos;
}
*/

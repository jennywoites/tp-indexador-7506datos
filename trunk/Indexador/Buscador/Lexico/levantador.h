#ifndef LEVANTADOR_H_
#define LEVANTADOR_H_
#define LEVANTADOR_OK 0
#define LEVANTADOR_ERROR 1
#include <stdbool.h>

/* DEFINICION DE FUNCION PARA GUARDAR DATOS */
typedef bool (*contenedor_guardar_dato_t)(void *, const char* , void*);


/* Funcion que permite almacenar el Lexico (proveniente de un archivo
 * con el Front Coding y otro con los diferentes) en un contenedor,
 * dada una funcion de almacenamiento.
 * Pre: las rutas son correctas.
 * Post: se almacenan los terminos, y se devuelve LEVANTADOR_OK en caso que haya salido bien, LEVANTADOR_ERROR en otro caso.
 * Memoria: Almacena dentro de la estructura todos los terminos (para los cuales se reserva memoria) */
int levantador_obtenerContenedorLexico(void* contenedor,contenedor_guardar_dato_t funcion ,const char* rutaFrontCoding, const char* rutaDiferentes);



#endif /* LEVANTADOR_H_ */

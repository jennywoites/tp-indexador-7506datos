#ifndef FUNCIONESGENERALESARCHIVOS_H_
#define FUNCIONESGENERALESARCHIVOS_H_
#include <stdbool.h>
#include <stdio.h>


//Permite hacer lectura anticipada, para casos de apareo y similares.
bool lectura_anticipada(FILE* arch, char* c);

const char* lectura_archivos();
const char* escritura_archivos();

//Obtiene un vector con lineas del archivo.
//Memoria: se guarda memoria para lo que se devuelve, y para cada linea guardada (cant)
char** obtenerLineas(FILE* archivo, unsigned int* cant);

//Obtiene una línea del archivo. Si no quedan líneas, se devuelve NULL.
//Memoria: se guarda memoria para la línea, si es que queda alguna.
char* obtenerLinea(FILE* archivo);


/* Permite crear rutas auxiliares a partir de una salida temporal base, un numero de archivo temporal,
 * y un numero maximo de archivos temporales a crear*/
char* __crear_ruta(unsigned int num, unsigned int maximo, const char* salida_temporal);

/* Realiza impresiones para mejorar la 'experiencia del usuario'*/
void emitir_impresion(const char* mensaje,size_t num, size_t total);

bool caracterDeSeparacion(char c);

bool caracterPrescindible(char c);

char* eliminarCaracteresPrescindibles(char* cadena, bool duplicante);

char** separarSiSonNumeros(char* buffer, unsigned int* cant);

void __toUPPERCase(char* cadena);

char* __obtenerNombreDoc(const char* paths, const char* offsets,  size_t num);

#endif /* FUNCIONESGENERALESARCHIVOS_H_ */

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

#endif /* FUNCIONESGENERALESARCHIVOS_H_ */

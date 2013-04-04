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

#endif /* FUNCIONESGENERALESARCHIVOS_H_ */

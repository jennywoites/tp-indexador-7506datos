#ifndef PARSER_INDEX_H
#define PARSER_INDEX_H

#define PARSERINDEX_OK 0
#define PARSERINDEX_ERROR 1

/* A partir de argv obtiene los dos parametros que se necesitan obtener.
 * En caso de haber mas, se devuelve error (cte PARSERINDEX_ERROR), sino, en 'cadenas'
 * quedara guardado en cada lugar (2) el valor de cada uno, siendo el primero el nombre
 * del repositorio, y el segundo el directorio de archivos a indexar, y devolvera PARSERINDEX_OK.
 * Precondicion: 'cadenas' tiene el lugar para (minimamente) de cadenas.
 * Memoria: en ambos lugares (2) dentro de 'cadenas' se guarda espacio para tener los dos parametros*/
int parserIndex_obtenerParametros(int argc, char** argv,char** cadenas);


/* A partir de la ruta del directorio que se quiere indexar, se obtienen la totalidad
 * de archivos que se tienen en el directorio (aclaracion: por ahora solo de ese directorio,
 * no de los sub-directorios).
 * Memoria: en rutas se guarda memoria para cant elementos, asi tambien como memoria para*/
int parserIndex_obtenerRutasDirectorios(char* directorio, char*** rutas, int* cant);

//Va a haber que ver bien como se hace este. TODAVIA NO ESTA TERMINADO, SOLO EMPEZADO
int parserIndex_parsearArchivo(const char* archivo);

#endif

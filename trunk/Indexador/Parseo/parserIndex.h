#ifndef PARSER_INDEX_H
#define PARSER_INDEX_H

#define PARSERINDEX_OK 0
#define PARSERINDEX_ERROR 1

/*A partir de argv obtiene los dos parametros que se necesitan obtener.
En caso de haber mas, se devuelve error (cte PARSERINDEX_ERROR), sino, en 'cadenas'
quedara guardado en cada lugar (2) el valor de cada uno, siendo el primero el nombre
del repositorio, y el segundo el directorio de archivos a indexar, y devolvera PARSERINDEX_OK.
Precondicion: 'cadenas' tiene el lugar para (minimamente) de cadenas.*/
int parserIndexObtenerParametros(int argc, char** argv,char** cadenas);


#endif

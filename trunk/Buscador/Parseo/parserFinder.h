#ifndef PARSER_FINDER_H
#define PARSER_FINDER_H

#define PARSERFINDER_OK 0
#define PARSERFINDER_ERROR 1


/*A partir de argv obtiene los dos parametros que se necesitan obtener.
En caso de haber mas, se devuelve error (cte PARSERFINDER_ERROR), sino, en 'cadenas'
quedará guardado en cada lugar (2) el valor de cada uno, siendo el primero el nombre
del repositorio, y el segundo la consulta a realizar, y devolvera PARSERFINDER_OK.
Precondicion: 'cadenas' tiene el lugar para (minimamente) de cadenas.*/
int parserFinder_obtenerParametros(int argc, char** argv,char** cadenas);



#endif



#ifndef SORTING_H_
#define SORTING_H_
#define SORTING_OK 0
#define SORTING_ERROR 1

/* A partir de un archivo de entrada, lo ordena a partir del concepto de registros
 * que se tienen. El ordenamiento utilizado es Replacement Selection, y se utiliza
 * un Merge de 2 etapas para unir las particiones ordenadas creadas.*/
int sorting_ordenarArchivo(const char* entrada, const char* salida);

#endif /* SORTING_H_ */

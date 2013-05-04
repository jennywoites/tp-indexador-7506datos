#ifndef INDEXER_H_
#define INDEXER_H_
#define INDEXER_OK 0
#define INDEXER_ERROR 1

/* Recibe un archivo de origen con el formato de salida del parseo/sort.
 * A partir de este archivo ordenado lee cada linea y a partir de las palabras
 * crea el lexico, y los punteros correspondientes.
 * El lexico se guardara en el archivo de ruta destino_lexico, y los punteros destino_index
 * Si salio bien el indexado, devuelvo INDEXER_OK, sino INDEXER_ERROR
 * Memoria: No guarda nada, solo crea los archivos correspondientes */
int indexer_indexar(const char* origen, const char* destino_index, const char* destino_lexico);



#endif /* INDEXER_H_ */

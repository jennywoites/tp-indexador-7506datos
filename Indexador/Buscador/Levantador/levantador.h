#ifndef LEVANTADOR_H_
#define LEVANTADOR_H_
#include "../../Carpetas Compartidas/TDAs/trie.h"
#include <stdio.h>


/* Funcion que permite obtener un trie con el lexico a partir
 * del archivo de front coding + los diferentes*/
trie_t* levantador_obtenerTrieLexico(FILE* archFrontCoding, FILE* archDiferentes);



#endif /* LEVANTADOR_H_ */

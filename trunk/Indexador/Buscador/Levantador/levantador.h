#ifndef LEVANTADOR_H_
#define LEVANTADOR_H_
#include "../../Carpetas Compartidas/TDAs/trie.h"


/* Funcion que permite obtener un trie con el lexico a partir
 * del archivo de front coding + los diferentes*/
trie_t* levantador_obtenerTrieLexico(const char* rutaFrontCoding, const char* rutaDiferentes);



#endif /* LEVANTADOR_H_ */

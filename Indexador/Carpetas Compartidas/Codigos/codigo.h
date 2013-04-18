#ifndef CODIGO_H_
#define CODIGO_H_

#define CODIGO_UNARIO 0
#define CODIGO_BINARIO 1
#define CODIGO_GAMMA 2
#define CODIGO_DELTA 3


typedef struct codigo codigo_t;

codigo_t* codigo_crear(unsigned int num, unsigned int tipo);

void codigo_destruir(codigo_t*);

//Para pruebas:
void codigo_imprimir(codigo_t*);
codigo_t* codigo_crearBinario(unsigned int num, unsigned  longitud );


#endif /* CODIGO_H_ */

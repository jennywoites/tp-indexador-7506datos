#include "trie.h"
#include <stdlib.h>
#include <string.h>

#define TRIE_AUMENTO_HIJOS 5
#define TRIE_NODO_POSIBLE 1
#define TRIE_NODO_IGUALES 0
#define TRIE_NODO_IMPOSIBLE 2
#define TRIE_DIFERENCIA 3
#define TRIE_CONTENIDO 2
#define TRIE_PADRE 1

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* Definicion de la estructura del nodo de un Trie*/
typedef struct nodo_trie{
	char* clave;
	void* dato;
	struct nodo_trie** hijos;
	size_t cant_hijos;
	size_t espacio_hijos;
	bool conDato;
}nodo_trie_t;

/* Defincion de la estructura del Trie*/
struct trie{
	nodo_trie_t* raiz;
	size_t cant;
	trie_destruir_dato_t fdestruir;
};


/* *****************************************************************
 *                    PRIMITIVAS DEL NODO DEL TRIE
 * *****************************************************************/
nodo_trie_t* nodo_trie_crear(const char* clave, void* dato){
	//if (!clave) return NULL;

	nodo_trie_t* nodo = malloc (sizeof(nodo_trie_t));
	if (!nodo) return NULL;
	nodo->hijos = malloc (sizeof(nodo_trie_t*) * TRIE_AUMENTO_HIJOS);
	if (!nodo->hijos){
		free(nodo);
		return NULL;
	}
	nodo->cant_hijos = 0;
	nodo->espacio_hijos = TRIE_AUMENTO_HIJOS;
	nodo->dato = dato;

	if (clave){
		nodo->clave = malloc (sizeof(char) * (strlen(clave)+1));
		if (!nodo->clave){
			free(nodo->hijos);
			free(nodo);
			return NULL;
		}
		strcpy(nodo->clave, clave);
	}else{
		nodo->clave = NULL;
	}
	nodo->conDato = true;
	return nodo;
}

void nodo_trie_destruir(nodo_trie_t* nodo, trie_destruir_dato_t fdest){
	if (!nodo) return;

	for (size_t i = 0; i < nodo->cant_hijos; i++){
		nodo_trie_destruir(nodo->hijos[i], fdest);
	}

	if (fdest)
		fdest(nodo->dato);
	free(nodo->hijos);
	if (nodo->clave)
		free(nodo->clave);
	free(nodo);
}

bool nodo_trie_agregar_hijo (nodo_trie_t* padre, nodo_trie_t* hijo){
	if (!padre || !hijo) return false;
	if (padre->espacio_hijos == padre->cant_hijos){
		size_t tam = padre->espacio_hijos + TRIE_AUMENTO_HIJOS;
		nodo_trie_t** aux = realloc (padre->hijos, sizeof(nodo_trie_t*) * tam);
		if (!aux) return false;
		padre->espacio_hijos = tam;
		padre->hijos = aux;
	}

	padre->hijos[padre->cant_hijos] = hijo;
	padre->cant_hijos++;
	return true;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL TREI
 * *****************************************************************/
nodo_trie_t* nodo_trie_siguiente(nodo_trie_t* padre, const char* clave, size_t pos, size_t* num);
nodo_trie_t* trie_conseguir_nodo_rec(nodo_trie_t* nodo, const char* clave, size_t actual);
char* concatenar_claves(const char*, size_t cant,const char*);
int comparacionPosibilidadClaves(const char* clave,const char* cmp);
void obtenerCadenaIgualesYDistintos(const char*, const char*, char**, char**, char**);
bool trie_agregarNodo_rec(nodo_trie_t* padre, nodo_trie_t* hijo, size_t num_hijo, char* clave, void* dato);
char* sacarDiferenciaClaves(char* exc, char* base);

trie_t* trie_crear(trie_destruir_dato_t dest){
	trie_t* trie = malloc (sizeof(trie_t));
	if (!trie) return NULL;

	trie->cant = 0;
	trie->fdestruir = dest;
	trie->raiz = nodo_trie_crear(NULL, NULL); //hay que tener cuidado al destruir
	trie->raiz->conDato = false;

	return trie;
}


void trie_destruir(trie_t* trie){
	if (!trie) return;

	for (size_t i = 0; i < trie->raiz->cant_hijos;i++){
		nodo_trie_destruir(trie->raiz->hijos[i], trie->fdestruir);
	}
	free(trie->raiz->hijos);
	free(trie->raiz);
	free(trie);
}


size_t trie_cantidad(const trie_t* trie){
	if (!trie) return 0;

	return trie->cant;
}

nodo_trie_t* trie_conseguir_nodo(const trie_t* trie, const char* clave);

bool trie_pertenece(const trie_t* trie, const char* clave){
	nodo_trie_t* nodo = trie_conseguir_nodo(trie, clave);
	if (!nodo) return false;
	return nodo->conDato;
}

void* trie_obtener(const trie_t* trie, const char* clave){
	nodo_trie_t* nodo = trie_conseguir_nodo(trie, clave);
	if (!nodo) return NULL;
	if (!nodo->conDato) return NULL;
	return nodo->dato;
}

bool trie_guardar(trie_t* trie, const char* clave, void* dato){
	if (!trie) return false;
	if (!clave) return false;

	//Veo si hay que actualizar, en ese caso no debo sumar nada:
	nodo_trie_t* nodo = trie_conseguir_nodo (trie, clave);
	if (nodo != NULL){
		if (trie->fdestruir)
			trie->fdestruir(nodo->dato);
		nodo->dato = dato;
		nodo->conDato = true;
		return true;
	}

	size_t num;
	nodo_trie_t* siguiente = nodo_trie_siguiente(trie->raiz, clave, 0, &num);
	//No hay siguiente -> no hay palabra que empiece con la misma letra:
	if (!siguiente){
		nodo_trie_t* nuevo = nodo_trie_crear(clave, dato);
		if( nodo_trie_agregar_hijo(trie->raiz, nuevo)){
			trie->cant++;
			return true;
		}else{
			return false;
		}
	}

	char* claveCpy = malloc (sizeof(char) * (strlen(clave)+1));
	strcpy(claveCpy, clave);
	if( trie_agregarNodo_rec(trie->raiz, siguiente, num, claveCpy, dato)){
		trie->cant++;
		return true;
	}else{
		return false;
	}
	
}


/* *****************************************************************
 *                    FUNCIONES AUXILIARES DEL TREI
 * *****************************************************************/


nodo_trie_t* trie_conseguir_nodo(const trie_t* trie, const char* clave){
	if (!trie || !clave) return NULL;

	nodo_trie_t* hijo_correcto = nodo_trie_siguiente(trie->raiz, clave, 0,NULL);
	if (!hijo_correcto) return NULL;
	
	return trie_conseguir_nodo_rec(hijo_correcto, clave, 0);
}


nodo_trie_t* nodo_trie_siguiente(nodo_trie_t* padre, const char* clave, size_t pos, size_t* num_hijo){
	for (size_t i = 0; i < padre->cant_hijos; i++)
		if (padre->hijos[i]->clave[0] == clave[pos]){
			if (num_hijo != NULL)
				*num_hijo = i;
			return padre->hijos[i];
		}
	return NULL;
}

nodo_trie_t* trie_conseguir_nodo_rec (nodo_trie_t* nodo, const char* clave, size_t actual){
	if (!nodo) return NULL;

	char* nueva = concatenar_claves(clave, actual, nodo->clave);

	int posibilidad_nodo = comparacionPosibilidadClaves(clave, nueva);
	size_t pos_ac = strlen(nueva);
	free(nueva);

	if (posibilidad_nodo == TRIE_NODO_IMPOSIBLE){
		return NULL;
	}

	if (posibilidad_nodo == TRIE_NODO_IGUALES){
		return nodo;
	}

	//Else: posibilidad_nodo == TRIE_NODO_POSIBLE

	nodo_trie_t* hijo_elegido = nodo_trie_siguiente(nodo,clave, pos_ac, NULL);
	if (!hijo_elegido) return NULL;

	return trie_conseguir_nodo_rec(hijo_elegido, clave, pos_ac);
}


char* concatenar_claves(const char* clave, size_t cant, const char* concatenator){
	char* resul = malloc (sizeof(char) * (cant + strlen(concatenator) + 1));

	for (size_t i = 0; i < cant; i++)
		resul[i] = clave[i];
	for (size_t i = cant; i <= (cant + strlen(concatenator));i++)
		resul[i] = concatenator[i-cant];

	return resul;
}

int comparacionPosibilidadClaves(const char* clave,const char* cmp){
	if (strcmp(clave, cmp) == 0) return TRIE_NODO_IGUALES;

	if (strlen(cmp) > strlen(clave)) return TRIE_NODO_IMPOSIBLE;

	bool iguales = true;
	for (size_t i = 0; (i < strlen(cmp)) && iguales; i++){
		iguales &= (cmp[i] == clave[i]);
	}
	if (iguales) return TRIE_NODO_POSIBLE;

	return TRIE_NODO_IMPOSIBLE;
}

int nodo_trie_relacion (const char* a, const char* b);

bool trie_agregarNodo_rec(nodo_trie_t* padre, nodo_trie_t* hijo, size_t num_hijo, char* clave, void* dato){
	if (!padre || !hijo || !clave) return false;

	/* Tres posibilidades:
	 	1) la nueva clave incluye el luegar en el que estamos. Estamos en el lugar correcto, hay que seguir
		2) la nueva clave esta incluida dentro del que estamos. Hay que crear un nuevo padre. Con dato = a guardar
		3) la nueva clave esta incluida pero con diferencias. Hay que crear un nuevo padre sin dato, con dos hijos: uno con cada diferencia.
	*/
	
	int relacion = nodo_trie_relacion (clave, hijo->clave);

	//Caso 1:

	if (relacion == TRIE_CONTENIDO){
		size_t num;
		char* sacado = sacarDiferenciaClaves(clave, hijo->clave);
		nodo_trie_t* siguiente = nodo_trie_siguiente(hijo, sacado, 0, &num);
		free(clave);
		if (!siguiente){
			nodo_trie_t* nuevo = nodo_trie_crear(sacado, dato);
			free(sacado);
			if (!nuevo) return false;
			return nodo_trie_agregar_hijo(hijo, nuevo);
		}
		return trie_agregarNodo_rec(hijo, siguiente, num, sacado, dato);
	}

	//Caso 2:
	if (relacion == TRIE_PADRE){
		char* sacado = sacarDiferenciaClaves(hijo->clave, clave);
		nodo_trie_t* nuevo = nodo_trie_crear(clave, dato);
		free(clave);
		if (!nuevo) return false;
		bool correcto = nodo_trie_agregar_hijo(nuevo, hijo);		
		if (!correcto) {nodo_trie_destruir(nuevo,NULL); return false;}
		
		free(hijo->clave);
		hijo->clave = sacado;
		padre->hijos[num_hijo] = nuevo;
		return true;
	}

	//Caso 3:
	if (relacion == TRIE_DIFERENCIA){
		char* iguales;
		char* distintos1, *distintos2;
		obtenerCadenaIgualesYDistintos(clave, hijo->clave, &iguales, &distintos1, &distintos2);
		free(clave);
		nodo_trie_t* nuevo_padre = nodo_trie_crear(iguales, NULL);
		free(iguales);

		nuevo_padre->conDato = false;
		nodo_trie_t* nuevo_hijo = nodo_trie_crear(distintos1, dato);
		free(distintos1);
		
		nodo_trie_agregar_hijo (nuevo_padre, nuevo_hijo);
		nodo_trie_agregar_hijo (nuevo_padre, hijo);
		padre->hijos[num_hijo] = nuevo_padre;
		free(hijo->clave);
		hijo->clave = distintos2;
		return true;
	}
	return true;
}

int nodo_trie_relacion (const char* a, const char* b){
	
	bool ok = true;
	size_t i;
	for (i = 0; i < strlen(a) && i < strlen(b) && ok; i++){
		ok = a[i] == b[i];
		if (!ok)
			i--;
	}
	if (i < strlen(a) && i < strlen(b)){
		return TRIE_DIFERENCIA;
	}
	
	if (i == strlen(a))
		return TRIE_PADRE;

	if (i == strlen(b))
		return TRIE_CONTENIDO;

	return 0;
}


void obtenerCadenaIgualesYDistintos(const char* or1, const char* or2, char** iguales, char** dist1, char** dist2){
	size_t i = 0;
	size_t min;
	if (strlen(or1) < strlen(or2))
		min = strlen(or1);
	else
		min = strlen(or2);
	char* ig = malloc (sizeof(char) * (min+1));
	while ( or1[i] == or2[i] && (i < min)){
		ig[i] = or1[i];
		i++;
	}
	ig[i] = '\0';
	*iguales = ig;


	char* d1 = malloc (sizeof(char) * (strlen(or1)+1));
	for (size_t k = i; k <= strlen(or1);k++)
		d1[k-i] = or1[k];

	char* d2 = malloc (sizeof(char) * (strlen(or2)+1));
	for (size_t k = i; k <= strlen(or2);k++)
		d2[k-i] = or2[k];

	*dist1 = d1;
	*dist2 = d2;
}

char* sacarDiferenciaClaves(char* exc, char* base){
	if (strlen(exc) < strlen(base)) return NULL;
	char* resul = malloc (sizeof(char) * (strlen(exc)+1));
	size_t i = 0;
	while (exc[i] == base[i]) i++;
	
	for (size_t j = i; j <= strlen(exc);j++)
		resul[j-i] = exc[j];
	
	return resul;
}

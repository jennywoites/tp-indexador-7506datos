// ARCHIVO DEL TIPO DE DATO HASH (hash.c)

//Se comporta como un Hashing de tipo ABIERTO
//Por lo tanto, necesito la estructura lista para poder implementarlo
#include "hash.h"
#include "lista.h"
#include <string.h>
#define STDTAM 127
#define NO_ELIMINAR_DATO false
#define ELIMINAR_DATO true

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/ 

/* Estructura del tipo HASH */

struct hash{
	lista_t** vector;
	size_t tam;
	size_t cantidad;
	hash_destruir_dato_t funcion_destruir;
};

/* Estructura del dato a guardar */
typedef struct paquete{
	char* clave;
	void* dato;
}paquete_t;

/* Estructura del iterador de hash */
struct hash_iter{
	lista_iter_t* iterador;
	size_t indice;
	const hash_t* hash;
};

/* ******************************************************************
 *                    PRIMITIVAS DEL PAQUETE 
 * *****************************************************************/

 // Crea un paquete con la clave y valor indicados.
 // Pre: -
 // Post: Devuelve el paquete con la clave y valor indicados.
 // Si no se pudo crear devuelve NULL.
paquete_t* paquete_crear(const char* key, void* valor){
	paquete_t* paquete=malloc(sizeof(paquete_t));
	if (!paquete) return NULL;

	paquete->clave=malloc(sizeof(char)*(strlen(key)+1));
	if(!paquete->clave){
		free(paquete);
		return NULL;
	}
	//se guarda una copia de la clave
	strcpy(paquete->clave, key);
	//guarda el dato
	paquete->dato=valor;
	return paquete;
}

// Destruye el paquete. Recibe si se desea eliminar el dato o no.
// Pre: -
// Post: elimina el paquete. Respeta si se pidio destruir el dato o no.
void paquete_destruir(hash_t* hash, paquete_t* paquete, bool eliminarDato){
	free(paquete->clave);
	//destruye el dato, si es requerido y hay funcion de destruccion
	if (eliminarDato && hash->funcion_destruir)
		hash->funcion_destruir(paquete->dato);

	free(paquete);
}

/* ******************************************************************
 *                    PRIMITIVAS DEL HASH 
 * *****************************************************************/

//Crea un nuevo hash, recibiendo la funcion que le permita destruir los
//datos que guarda (puede recibir NULL)
hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	//aparta memoria para la estructura
	hash_t* hash=malloc(sizeof(hash_t));
	if(!hash) return NULL;
	
	//memoria para el vector
	hash->vector=malloc(sizeof(lista_t*)*STDTAM);
	if(!hash->vector){
		free(hash);
		return NULL;
	}
	int i,j;
	//crea una lista en cada posicion del vector
	for (i=0; i<STDTAM;i++){
		hash->vector[i]=lista_crear();        
		if(!hash->vector[i]){
			for (j=0;j<i;j++)
				lista_destruir(hash->vector[j],NULL);
			free(hash->vector);
			free(hash);
			return NULL;
		}	
    }

	hash->tam=STDTAM;
	hash->cantidad=0;
	hash->funcion_destruir=destruir_dato;
	return hash;
}

//Se destruyen los datos del hash, y el mismo
void hash_destruir(hash_t* hash){
	int i;
	paquete_t* paq;
	//pasa por todas las listas:
	for (i=0; i<hash->tam; i++){
		//vaciando cada una de ellas:
		while(!lista_esta_vacia(hash->vector[i])){
			paq=lista_borrar_primero(hash->vector[i]);
			paquete_destruir(hash,paq,ELIMINAR_DATO);
		}
		//y finalmente las destruye:
		lista_destruir(hash->vector[i],NULL);
	}
	free(hash->vector);
	free(hash);
}


int funcion_de_hashing(const char *key,size_t largo){
	int hash, i;
	for(hash = i = 0; i < strlen(key); ++i){
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	hash %= largo;
	return hash;
}

//Funcion que redimensiona el Hash
void rehashing(hash_t* hash, size_t nuevoTam){
	//Creo un nuevo vector
	lista_t** vectorNuevo=malloc(sizeof(lista_t*)*nuevoTam);
	if(!vectorNuevo) return;

	int j,i;
	for (j=0; j<nuevoTam;j++){
		vectorNuevo[j]=lista_crear();        
		if (!vectorNuevo[j]){
			for (i=0; i<j; i++)
				lista_destruir(vectorNuevo[i],NULL);
			free(vectorNuevo);
			return;
		}
	}

	paquete_t* paquete;

	int pos;
	//paso todos los paquetes que tenia en el vector viejo al nuevo
	//y voy destruyendo las listas cuando queden vacias
	for (i=0; i<hash->tam; i++){
		while(!lista_esta_vacia(hash->vector[i])){
			paquete = lista_borrar_primero(hash->vector[i]);
			if(paquete!=NULL){
					pos=funcion_de_hashing(paquete->clave, nuevoTam);
					lista_insertar_primero(vectorNuevo[pos],paquete);
			}
		}
		lista_destruir(hash->vector[i],NULL);
	}

	//Libero la lista anterior y le asigno la nueva, se hace un 
	//reemplazo del vector
	free(hash->vector);
	hash->vector=vectorNuevo;
	hash->tam=nuevoTam;
}

//Decide si debe hacerse un rehashing
void rehashing_verificar(hash_t* hash){
	//se escogio un criterio basado en la relacion entre el tamanio y
	//la cantidad de elementos en el hash
	if(hash->cantidad>=(hash->tam*2))
		rehashing(hash,hash->tam*4);
	else
		if (hash->cantidad<=(0.15*hash->tam) && hash->tam> STDTAM)
			rehashing(hash,hash->tam/2);        
}

//Busca un paquete dentro de una lista
paquete_t* paquete_busqueda(lista_t* lista,const char* clave){
	paquete_t* paqueteAux;
	lista_iter_t* iter=lista_iter_crear(lista);
	bool encontre=false;
	//para todos los elementos de la lista
	while (!lista_iter_al_final(iter) && !encontre){
		paqueteAux=lista_iter_ver_actual(iter);
		//compara las claves
		encontre=strcmp(clave,paqueteAux->clave)==0;
		if (!encontre)	lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	if (encontre)	return paqueteAux;
	else return NULL;
}

//Guarda o actualiza un valor en el hash a partir de la clave y el dato
// a guardar
bool hash_guardar(hash_t* hash, const char* clave, void* dato){
	if(!hash) return false;

	//pide a la funcion de hashing la posicion a la que corresponde 
	//la clave
	int pos=funcion_de_hashing(clave, hash->tam);
	
    paquete_t* paquete=paquete_busqueda(hash->vector[pos],clave);
	bool rta;
	
	//si hay un paquete:
	if (paquete!=NULL){
		//actualiza el valor
		if(hash->funcion_destruir)	//hay funcion de destruccion, aplica
			hash->funcion_destruir(paquete->dato);
		paquete->dato=dato;
		rta=true;
	}else{	//sino:
		//guarda el valor
		paquete_t* aux=paquete_crear(clave, dato);
		rta=lista_insertar_ultimo(hash->vector[pos],aux);
		if(rta) hash->cantidad++;
	}

	rehashing_verificar(hash);

	return rta;
}

//Informa si una clave esta dentro del Hash
bool hash_pertenece(const hash_t* hash, const char* clave){
	if(!hash) return false;
	//busca segun la posicion que da la funcion de hashing
	int pos=funcion_de_hashing(clave, hash->tam);
	
    paquete_t* paquete=paquete_busqueda(hash->vector[pos],clave);
	return paquete!=NULL;
}

//Elimina del hash una clave, y le devuelve el dato al usuario
void* hash_borrar(hash_t* hash, const char* clave){
	if (!hash) return false;
	
	if(!hash_pertenece(hash,clave)) return false;
	//busca segun la posicion que da la funcion de hashing
	int pos=funcion_de_hashing(clave, hash->tam);

    lista_iter_t* iter= lista_iter_crear(hash->vector[pos]);
	paquete_t* paquete=lista_iter_ver_actual(iter);
	bool encontrado=strcmp(clave,paquete->clave)==0;

	//Ya se que debe estar en esta lista, ahora se busca donde
	while(!encontrado){
		lista_iter_avanzar(iter);
		paquete=lista_iter_ver_actual(iter);
		encontrado=strcmp(clave, paquete->clave)==0;
	}
    
	// Ya lo tengo, ahora lo borro
	paquete=lista_borrar(hash->vector[pos],iter);
	void* devolver=paquete->dato;
	paquete_destruir(hash,paquete,NO_ELIMINAR_DATO);
	hash->cantidad--;
	lista_iter_destruir(iter);

	rehashing_verificar(hash);
	return devolver;
}

//Obtiene el dato de una clave especifica
void* hash_obtener(const hash_t* hash, const char* clave){

	if(!hash) return NULL;
	//busca segun la posicion que da la funcion de hashing
    int pos=funcion_de_hashing(clave, hash->tam);
    paquete_t* aux=paquete_busqueda(hash->vector[pos],clave);
    
    if (!aux) return NULL;
    
	return aux->dato;
}

//Devuelve la cantidad de elementos en el hash
size_t hash_cantidad(const hash_t* hash){
	if (!hash) return 0;

	return hash->cantidad;
}


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR DE HASH
 * *****************************************************************/

//Crea un nuevo iterador de hash
hash_iter_t* hash_iter_crear(const hash_t* hash){
	if (!hash) return NULL;

	hash_iter_t* iter=malloc(sizeof(hash_iter_t));
	iter->indice=0;

	if(hash_cantidad(hash)==0) {
		iter->iterador=lista_iter_crear(hash->vector[0]);
		return iter;
	}

	//Me posiciono en la primera lista que tenga algo
	while(lista_esta_vacia(hash->vector[iter->indice]) && iter->indice < hash->tam)
		iter->indice++;

	//Si el hash esta vacio lo posiciono en la ultima lista
	if (iter->indice==hash->tam){
		iter->indice--;
	}

	//Creo el iterador de lista
	iter->iterador=lista_iter_crear(hash->vector[iter->indice]);

	if (!iter->iterador){
		free(iter);
		return NULL;
	}
	iter->hash=hash;             
	return iter;
}

//El iterador avanza e informa si lo ha logrado con exito
bool hash_iter_avanzar(hash_iter_t* iter){
	if (!iter) return false;
	
	//si estoy al final no puedo avanzar
	if (hash_iter_al_final(iter)) return false;

	lista_iter_avanzar(iter->iterador);
	//si en la lista en la que estoy hay otro elemento, es el que debo
	// usar ahora
	if (!lista_iter_al_final(iter->iterador))
		return true;

    //aumento mi posicion en el hash hasta llegar a una lista con
	//elementos o a la ultima posicion valida del hash
	do
		iter->indice++;
	while(lista_esta_vacia(iter->hash->vector[iter->indice]) && iter->indice < (iter->hash->tam-1));

	lista_iter_destruir(iter->iterador);
	iter->iterador=lista_iter_crear(iter->hash->vector[iter->indice]);
    
	return !hash_iter_al_final(iter);	//devuelve true solo si, tras 
										//avanzar, logramos estar en 
										//una posicion valida del hash
}

//Se devuelve el valor de la clave en la que esta situado el iterador
const char* hash_iter_ver_actual(const hash_iter_t* iter){
	if (!iter) return NULL;

	if(hash_iter_al_final(iter)) return NULL;

	paquete_t* paquete=lista_iter_ver_actual(iter->iterador);

	return paquete->clave;
}

//LA UNICA FORMA DE QUE EL ITERADOR DEL ITER ESTE AL FINAL ES HABER 
//LLEGADO
//AL FINAL DEL HASH (SINO, CUANDO AVANZO LO MANDO AL PROXIMO INDICE 
//POSIBLE)
bool hash_iter_al_final(const hash_iter_t* iter){
	return lista_iter_al_final(iter->iterador);
}

//Destruye el iterador de Hash
void hash_iter_destruir(hash_iter_t* iter){
	lista_iter_destruir(iter->iterador);
	free(iter);
}

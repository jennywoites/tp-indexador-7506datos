#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "interaccion.h"

char* leer_texto(){
	char c;	size_t tam = 10; int i;  // tam = 10 es completamente arbitrario

	char* buffer = malloc(sizeof(char)*(tam));
	if (!buffer) return NULL;

	for(i = 0; (c = getchar()) != '\n'; i++){ //leo hasta el enter

		if(i + 1 == tam){ // si llego al tamanio de mi cadena
			tam += tam;  // la forma de modificar tam es arbitraria
			char* aux = realloc(buffer,tam * sizeof(char));
			if(!aux) break;  // si falla realloc, termino aca
			buffer = aux;
		}

		buffer[i] = c;
	}
	buffer[i]='\0'; // es necesario cerrar el string
	return buffer;
}

int pot_interaccion(int x, int y){
	if (y==0) return 1;
	if (y==1) return x;
	
	int h=pot_interaccion(x, y/2);
	int w;
	if (y%2==0) w=1;
	else w=x;
	
	return h*h*w;
	}

int leer_numero(){
	
	char c;	size_t tam = 6; int i;  // tam = 10 es completamente arbitrario
	
	char* buffer = malloc(sizeof(char)*(tam));
	if (!buffer) return -1;
	
	for(i = 0; (c = getchar()) != '\n'; i++){ //leo hasta el enter
		
		if(i + 1 == tam){ // si llego al tamanio de mi cadena
			//tam += tam;  // la forma de modificar tam es arbitraria
			char* aux = realloc(buffer,tam * sizeof(char));
			if(!aux) break;  // si falla realloc, termino aca
			buffer = aux;
		}
		buffer[i] = c;
	}
	buffer[i]='\0'; // es necesario cerrar el string
    int numero,j;
    bool sonNumeros=true;
    numero=0; j=0;
    
    //Me fijo que tenga todos numeros
    while (j<i && sonNumeros){
          sonNumeros=(buffer[j]>='0' && buffer[j]<='9');
          j++;
          }
    
    if (!sonNumeros){
	   free( buffer);
	   return -1;
       }
     
    //Ahora que se que son numeros, lo paso a un integer
    for (j=0; j<i;j++)
        numero+=(buffer[j]-'0')*pot_interaccion(10,i-j-1);
         
    free(buffer);
    return numero;     // la funcion tiene un peque�o error con numeros muy grandes, pero como estos no nos importan
                       // ni nos vamos a fijar
}

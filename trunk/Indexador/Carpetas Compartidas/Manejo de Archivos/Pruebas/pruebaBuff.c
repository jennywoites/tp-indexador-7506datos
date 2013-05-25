#include "../buffer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main (){
	FILE* file = fopen("buff.jem","w");
	buffer_t* buff = buffer_crear(file);
	
	buffer_escribir_byte(buff,70);
	
	//buffer_escribir_LS_bits(buff,2,3);
	
	buffer_escribir_MS_bits(buff,64,3);
	
	buffer_escribir_byte(buff,138);

	buffer_escribir_bit(buff,0);
	buffer_escribir_bit(buff,0);
	buffer_escribir_bit(buff,1);
	buffer_escribir_bit(buff,0);
	buffer_escribir_bit(buff,0);
	
	Byte_t vector[] = {65,67,69};
	buffer_escribir_bytes(buff,vector,3);
	
	buffer_destruir(buff);
	
	fclose(file);
	return 0;
}

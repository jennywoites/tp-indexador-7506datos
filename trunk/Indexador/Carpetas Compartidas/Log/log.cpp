#include "log.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

const char* LOG_ARCHIVO = "log.jem";

void log_emitir(std::string mensaje){

	if (fork() != 0)
		return;
	int salida = open(LOG_ARCHIVO, O_WRONLY);
	dup2(salida,1);

	while (!std::cin.eof()){
		char letra;
		std::cin >> letra;
		std::cout << letra;
	}
	exit(0);
}


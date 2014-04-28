#ifndef LOG_H_
#define LOG_H_
#define DEBUG 0
#define RELEASE 1

#include <string>

/* Explicacion: Las entradas al log pueden ser de distinto tipo:
 * - Informativas (comunes)
 * - Informativas importantes (que informan sobre aspectos importantes a guardar)
 * - Errores ocurridos
 * - Datos o detalles de procesos realizados
 *
 * Hay distintos modos para el Log.
 * - Nada: no imprime nada, en ningun caso
 * - Completo: imprime absolutamente todos los rotulos
 * - Release: solo imprime la informacion importante y los errores
 * - Debug de Errores: solo se imprimen errores.
 * - Debug de Errores y procesos: solo imprime errores y detalles de procesos (sirve
 * 		(cuando el error no es suficientemente descriptivo y queremos conocer mejor el
 * 		contexto).
 * */

#define LOG_MODO DEBUG

void log_emitir(std::string mensaje, unsigned int tipo);

#endif /* LOG_H_ */

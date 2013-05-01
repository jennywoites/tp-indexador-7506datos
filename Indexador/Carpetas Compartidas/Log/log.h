#ifndef LOG_H_
#define LOG_H_
#define LOG_NADA 0
#define LOG_COMPLETO 1
#define LOG_RELEASE 2
#define LOG_DEBUG_ERRORES 3
#define LOG_DEBUG_ERRORES_Y_PROCESOS 4

#define LOG_ENTRADA_INFORMATIVA 1
#define LOG_ENTRADA_INFORMATIVA_IMPORTANTE 2
#define LOG_ENTRADA_ERROR 3
#define LOG_ENTRADA_PROCESO 4

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

#define LOG_MODO LOG_COMPLETO

void log_emitir(const char* mensaje, unsigned int tipo);

#endif /* LOG_H_ */

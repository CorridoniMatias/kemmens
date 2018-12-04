#ifndef KEMMENS_UTILS_H_
#define KEMMENS_UTILS_H_

//Descomentar la siguiente linea para activar el util de md5, requiere incluir la biblioteca -lcrypto en el proyecto donde se usen las kemmens
//#define ENABLE_MD5

#ifdef ENABLE_MD5
#include <openssl/md5.h>
#endif

#include <stdarg.h>
#include <stdlib.h>
#include "commons/collections/dictionary.h"
#include "commons/collections/list.h"
#include "commons/collections/queue.h"
#include "StringUtils.h"

//Define un puntero con el tipo de datos indicado y el valor inicial provisto. NO SIRVE PARA char*
#define declare_and_init(pointer_name, data_type, init_value) 								\
		data_type* pointer_name = (data_type*)malloc(sizeof(data_type)); 					\
		*pointer_name = init_value;


#ifdef ENABLE_MD5

/**
 * 		Crea un hash md5 para el void* provisto.
 *
 * 		Ejemplo:
 *
 * 		unsigned char digest[MD5_DIGEST_LENGTH];

		md5("hola!", digest); //digest va a tener el hash

		for(int x = 0; x < MD5_DIGEST_LENGTH; x++)
		        printf("%02x", digest[x]); 	//imprime por pantalla el hash

 *
 */
void md5(void* content, unsigned char* digest);

#endif


/*
 * 	ACCION: Poner un key-value en un diccionario y si ya existia la key borrar el
 * 	value anterior usando elementDestroyer para evitar un memory leak.
 * 	PARAMETROS:
 * 		dictionary: Diccionario donde guardar el key-value
 * 		key: Key para verificar si ya existe y para guardar en el diccionario.
 * 		value: Valor a guardar en el diccionario asociado a Key.
 * 		elementDestroyer: Función para usar en caso que ya exista esa Key y asi liberar
 * 		el value anterior.
 */
void dictionary_putMAESTRO(t_dictionary* dictionary, char* key, void* value, void (*elementDestroyer)(void*));

/*
 * 	ACCION: Copiar el contenido de una lista a otra (no hace create, a diferencia de list_duplicate)
 * 	PARAMETROS
 * 		src: Lista que tomo de fuente
 * 		dst: Lista que sirve como destino
 *
 * 	NOTA: Si armo una lista con esta funcion, solo la destruyo; no a sus elementos, ya que tienen los mismos
 * 		  punteros que la lista original; podria hacer un list_copy de todo al final y garantizar no pifiarla
 */
void list_copy(t_list* src, t_list* dest);

/*
 * 	ACCION: Obtener una lista a partir de una cola, sin borrar la cola original
 * 	PARAMETROS:
 * 		src: Cola que sirve de base o fuente de la copia
 * 		dest: Lista a donde quiero copiar los elementos, destino
 *
 * 	NOTA: Se usa para MoveQueues; el free de los elementos deberia ser en el delete final de variables globales
 */
void queue_to_list(t_queue* src, t_list* dest);

/*
 * 	ACCION: Obtener una cola a partir de una lista, con sus mismos elementos
 * 	PARAMETROS:
 * 		src: Lista que sirve de base o fuente de la copia
 * 		dest: Cola a donde quiero copiar los elementos, destino
 */
void list_to_queue(t_list* src, t_queue* dest);

/*
 * 	ACCION: Hacer varios free en una sola llamada, con una lista variable de argumentos
 * 	PARAMETROS:
 * 		pointersCount (fijo): Cantidad de punteros a liberar
 */
void multiFree(int pointersCount, ...);

#endif /* KEMMENS_UTILS_H_ */

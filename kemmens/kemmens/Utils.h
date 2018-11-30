#ifndef KEMMENS_UTILS_H_
#define KEMMENS_UTILS_H_

#include <openssl/md5.h>
#include "commons/collections/dictionary.h"

#define declare_and_init(pointer_name, data_type, init_value) 								\
		data_type* pointer_name = (data_type*)malloc(sizeof(data_type)); 					\
		*pointer_name = init_value;															\



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

#endif /* KEMMENS_UTILS_H_ */

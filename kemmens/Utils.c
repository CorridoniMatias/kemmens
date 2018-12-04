#include "kemmens/Utils.h"

#ifdef ENABLE_MD5

void md5(void* content, unsigned char* digest)
{
	MD5_CTX context;
	MD5_Init(&context);
	MD5_Update(&context, content, strlen(content) + 1);
	MD5_Final(digest, &context);
}

#endif

void dictionary_putMAESTRO(t_dictionary* dictionary, char* key, void* value, void (*elementDestroyer)(void*))
{
	if (dictionary_has_key(dictionary, key)) {
		dictionary_remove_and_destroy(dictionary, key, elementDestroyer);
	}
	dictionary_put(dictionary, key, value);
}

void list_copy(t_list* src, t_list* dest)
{

	//Limpio la lista de destino, asi me aseguro de sobreescribirla; ignoro lo que habia antes
	list_clean(dest);

	int listSize = list_size(src);
	int pos;

	for (pos = 0; pos < listSize; pos++)
	{
		list_add(dest, list_get(src, pos));
	}

	return;

}

void queue_to_list(t_queue* src, t_list* dest)
{

	//Aprovecho que la cola tiene, internamente, una lista de elementos, y la duplico: obtengo una lista auxiliar
	t_list* aux = list_duplicate(src->elements);

	//Copio los elementos de esa lista auxiliar (la lista interna de la cola) a la lista destino de la funcion
	list_copy(aux, dest);

	//Destruyo la lista auxiliar; no sus elementos, ya que haria un doble free al final, y perderia sus referencias
	list_destroy(aux);

	return;

}

void list_to_queue(t_list* src, t_queue* dest)
{

	//Vacio la cola de destino, para sobreescribirla por completo (no tomo en cuenta lo que tenia)
	queue_clean(dest);

	int listSize = list_size(src);
	int pos;

	for (pos = 0; pos < listSize; pos++)
	{
		queue_push(dest, list_get(src, pos));
	}

	return;

}

void multiFree(int pointersCount, ...)
{

	if(pointersCount < 1)
	{
		return;
	}

	va_list arguments;
	va_start(arguments, pointersCount);

	void* ptr;
	int current;

	for(current = 0; current < pointersCount; current++)
	{
		ptr = va_arg(arguments, void*);
		free(ptr);
	}

	va_end(arguments);

}

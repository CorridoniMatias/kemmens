#include "kemmens/Utils.h"

void dictionary_putMAESTRO(t_dictionary* dictionary, char* key, void* value, void (*elementDestroyer)(void*))
{
	if (dictionary_has_key(dictionary, key)) {
		dictionary_remove_and_destroy(dictionary, key, elementDestroyer);
	}
	dictionary_put(dictionary, key, value);
}
#include "kemmens/Utils.h"


unsigned char* KemmensUtils_md5(void* content)
{
	void * digest = malloc(MD5_DIGEST_LENGTH);
	MD5_CTX context;
	MD5_Init(&context);
	MD5_Update(&context, content, strlen(content) + 1);
	MD5_Final(digest, &context);

	return (char*)digest;
}

void dictionary_putMAESTRO(t_dictionary* dictionary, char* key, void* value, void (*elementDestroyer)(void*))
{
	if (dictionary_has_key(dictionary, key)) {
		dictionary_remove_and_destroy(dictionary, key, elementDestroyer);
	}
	dictionary_put(dictionary, key, value);
}
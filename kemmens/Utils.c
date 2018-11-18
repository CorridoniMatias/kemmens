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

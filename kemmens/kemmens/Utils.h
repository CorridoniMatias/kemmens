#ifndef KEMMENS_UTILS_H_
#define KEMMENS_UTILS_H_

#include <openssl/md5.h>

#define declare_and_init(pointer_name, data_type, init_value) 								\
		data_type* pointer_name = (data_type*)malloc(sizeof(data_type)); 					\
		*pointer_name = init_value;															\


unsigned char* KemmensUtils_md5(void* content);

#endif /* KEMMENS_UTILS_H_ */

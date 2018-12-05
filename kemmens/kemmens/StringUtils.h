#ifndef KEMMENS_STRINGUTILS_H_
#define KEMMENS_STRINGUTILS_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

//Para deshabilitar el logueo interno de este modulo de las kemmens comentar la siguiente linea:
#define STRINGUTILS_ENABLE_LOGGING


char* StringUtils_Format(char* plain, ...);

int StringUtils_ArrayContains(char** array, char* needle);

int StringUtils_ArraySize(char** array);

void StringUtils_FreeArray(char** array);

int StringUtils_IndexOf(char* str, int character);

int StringUtils_LastIndexOf(char* str, int character);


/*
 * Ejemplo:
 * int* t = malloc(4*4);
	t[0] = 1;
	t[1] = 2;
	t[2] = 3;
	t[3] = 4;

	char* c1 = StringUtils_ArrayFromInts(t, 4, true, true);
	char* c2 = StringUtils_ArrayFromInts(t, 4, false, true);
	char* c3 = StringUtils_ArrayFromInts(t, 4, true, false);
	char* c4 = StringUtils_ArrayFromInts(t, 4, false, false);

	printf("1: %s\n2:%s\n3:%s\n4:%s\n", c1,c2,c3,c4);

	free(c1);
	free(c2);
	free(c3);
	free(c4);
	free(t);
 *
 */
char* StringUtils_ArrayFromInts(int* ints, int intsCount, bool startingBracket, bool endingBracket);

int StringUtils_CountOccurrences(char* str, char* needle);

#endif /* KEMMENS_STRINGUTILS_H_ */

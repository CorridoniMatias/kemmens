#include <stdio.h>
#include <stdlib.h>
#include <parsi/parser.h>
#include <commons/collections/list.h>


#ifndef ESIPROCESS_LECTORARCHIVO_H_
#define ESIPROCESS_LECTORARCHIVO_H_

typedef struct {
    t_esi_operacion operacion;
} t_esi;

FILE* abrirArchivo(char*);
t_list* leerLineas(FILE*);
void esi_destroy(t_esi);
t_esi* esi_create(t_esi_operacion);
void limpiarLineas(FILE*,t_list*);




#endif /* ESIPROCESS_LECTORARCHIVO_H_ */

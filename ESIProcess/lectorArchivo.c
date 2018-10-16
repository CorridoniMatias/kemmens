#include "lectorArchivo.h"

#define EXIT_FAILURE 1


typedef struct {
    t_esi_operacion operacion;
} t_esi;

t_esi* esi_create(t_esi_operacion operacion) {
    t_esi *new = malloc(sizeof(t_esi_operacion));
    new->operacion = operacion;
    return new;
}
void esi_destroy(t_esi *self){
	destruir_operacion(self->operacion);
	free(self);
	return;
}

FILE* abrirArchivo(char* archivoLineas){
	FILE* archivo = fopen(archivoLineas,"r");
	if(archivo==NULL){
			perror("El archivo no se pudo abrir \n");
			exit(EXIT_FAILURE);
	}
	return archivo;
}

t_list* leerLineas(FILE* archivo){
	char* linea = NULL;
	size_t len = 0;
	t_list* lista_esi;
	lista_esi = list_create();
	while(getline(&linea,&len,archivo)!=-1){
			t_esi_operacion operacionEsi = parse(linea);
			if(true){
				printf("hola \n");
			}
			list_add(lista_esi,esi_create(operacionEsi));
	}
	if(linea){
		free(linea);
	}
	return lista_esi;
}

void limpiarLineas(FILE* f,t_list* listaEsis){
	fclose(f);
	list_destroy_and_destroy_elements(listaEsis,(void*)esi_destroy);
	return;
}


int main (int argc, char *argv[]){

}

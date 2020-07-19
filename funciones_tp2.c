#include "funciones_tp2.h"
#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include "heap.h"
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include <stdbool.h>
#include<stdlib.h>
#include <ctype.h>



/*struct de estructuras*/

struct estructuras{
	abb_t* doctores;
	hash_t* pacientes;
};

void destruir_estructura(estructuras_t* estructura){	
	if(estructura->pacientes) hash_destruir(estructura->pacientes);
	if(estructura->doctores) abb_destruir(estructura->doctores);
	free(estructura);
}

estructuras_t* estructuras_crear(){
	estructuras_t* estructura = malloc(sizeof(estructuras_t));
	if(!estructura) return NULL;	
	estructura->doctores = abb_crear(strcmp, destruir_campo_doctores); 
	if(!estructura->doctores){
		destruir_estructura(estructura);
		return NULL;
	}
	estructura->pacientes = hash_crear(free);
	if(!estructura->pacientes){
		destruir_estructura(estructura);
		return NULL;
	}
	return estructura;
}


/*struct campo_doctores*/

struct campo_doctores{
	char* especialidad;
	int atendidos;
};

campo_doctores_t* campo_doctores_crear(char* especialidad){
	campo_doctores_t* campo_doctores = malloc(sizeof(campo_doctores_t));
	if (!campo_doctores) return NULL;
	campo_doctores->especialidad = strdup(especialidad);
	if (!campo_doctores->especialidad){
		free(campo_doctores);
		return NULL;
	}
	campo_doctores->atendidos = 0;
	return campo_doctores;
}

void destruir_campo_doctores(void* campo_doctores){
	free(((campo_doctores_t*)campo_doctores)->especialidad);
	free((campo_doctores_t*)campo_doctores);
}

/*funciones auxiliares*/

bool es_numero(char* str){
	for (int x = 0; str[x] != '\0'; x++){
		if (!isdigit(str[x])) return false;
	}
	return true;
}




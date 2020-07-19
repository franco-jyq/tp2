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
#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define POSICION_ARCHIVO_DOCTORES 1
#define POSICION_ARCHIVO_PACIENTES 2



void procesar_comando(const char* comando, const char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {

	} else {

	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada() {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

/*
*
*					lo nuestro
*
**************************************************************************/


bool es_numero(char* str){
	for(int x = 0; str[x] != '\0'; x++){
		if(!isdigit(str[x])) return false;
	}
	return true;
}

typedef struct campo_doctores{
	char* especialidad;
	int atendidos;
}campo_doctores_t;

campo_doctores_t* campo_doctores_crear(char* especialidad){
	campo_doctores_t* campo_doctores = malloc(sizeof(campo_doctores_t*));
	if(!campo_doctores) return NULL;
	campo_doctores->especialidad = strdup(especialidad);
	if(!campo_doctores->especialidad){
		free(campo_doctores);
		return NULL;
	}
	campo_doctores->atendidos = 0;
	return campo_doctores;
}

void destruir_campo_doctores(campo_doctores_t* campo_doctores){
	free(campo_doctores->especialidad);
	free(campo_doctores);
}

typedef struct estructuras{
	abb_t* doctores;
	hash_t* pacientes;
} estructuras_t;

void estructura_destruir(estructuras_t* estructura){
	
	if(estructura->pacientes) hash_destruir(estructura->pacientes);
	if(estructura->doctores) abb_destruir(estructura->doctores);
	free(estructura);
}

estructuras_t* estructuras_crear(){
	estructuras_t* estructura = malloc(sizeof(estructuras_t));
	if(!estructura) return NULL;
	
	estructura->doctores = abb_crear(strcmp, destruir_campo_doctores); // aca va la funcion de destruccion
	if(!estructura->doctores){
		estructura_destruir(estructura);
		return NULL;
	}
	estructura->pacientes = hash_crear(free);
	if(!estructura->pacientes){
		estructura_destruir(estructura);
		return NULL;
	}

	return estructura;
}


estructuras_t* leo_archivos(char* archivo_doctores, char* archivo_pacientes){
	 
	FILE*  archivo = fopen(archivo_doctores, "r");
	if(!archivo) return NULL;

	estructuras_t* estructura = estructuras_crear();
	if(!estructura) return NULL;
	
	char* linea = NULL;
	size_t capacidad = 0;
	
	while(getline(&linea, &capacidad, archivo) > 0){
		char** campos = split(linea, ',');
		if(!campos){
			destruir_estructura(estructura);
			return NULL;
		}
		campo_doctores_t* campo_doctores = campo_doctores_crear(campos[0]);
		if(!campo_doctores){
			destruir_estructura(estructura);
			return NULL;
		}
		if(!abb_guardar(estructura->doctores, campos[0], campo_doctores)){
			destruir_estructura(estructura);
			destruir_campo_doctores(campo_doctores);
			return NULL;
		}
		free_strv(campos);
	}
	fclose(archivo);

	FILE*  archivo2 = fopen(archivo_pacientes, "r");
	if(!archivo){
		destruir_estructura(estructura);
		return NULL;
	}
	linea = NULL;
	capacidad = 0;
	
	while(getline(&linea, &capacidad, archivo2) > 0){
		char** campos = split(linea, ',');
		if(!campos || es_numero(campos[1])){
			destruir_estructura(estructura);
			return NULL;
		}
		int* numero = malloc(sizeof(int));
		if(!numero){
			destruir_estructura(estructura);
			return NULL;
		}
		*numero = atoi(campos[1]);
		if(!hash_guardar(estructura->pacientes, campos[0], numero)){
			destruir_estructura(estructura);
			free(numero);
			return NULL;
		}
		free_strv(campos);
	}
	fclose(archivo2);

	return estructura;
}

int main(int argc, char** argv) {
	
	if(argc != 3) return 1;

	char* archivo_doctores = argv[POSICION_ARCHIVO_DOCTORES];
	char* archivo_pacientes = argv[POSICION_ARCHIVO_PACIENTES];
	estructuras_t* estructura = leo_archivos(archivo_doctores, archivo_pacientes);
	if(!estructura) return 1;
			
	return 0;
}












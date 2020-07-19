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
#include <stdlib.h>
#include <ctype.h>
#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define POSICION_ARCHIVO_DOCTORES 1
#define POSICION_ARCHIVO_PACIENTES 2

/*Struct campo_doctores*/

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


/*Struct de estructuras*/

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


/*

void procesar_comando(const char* comando, const char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {

	} else {

	}
}
*/
void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}
/*
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
*/

/*
*
*					lo nuestro
*
**************************************************************************/

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
		eliminar_fin_linea(campos[1]);
		campo_doctores_t* campo_doctores = campo_doctores_crear(campos[1]);
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
	free(linea);
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
		if(!campos || !es_numero(campos[1])){ // sin todas esas lineas se pierde memoria
			puts("s"); // esta cayendo siempre aca no se porque
			free(linea);
			free_strv(campos);
			fclose(archivo2);               // hay que hacer una funcion auxiliar que ejecute estas 4 lineas
			destruir_estructura(estructura); // quizas meter todo aca
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
	free(linea);
	return estructura;
}

// funciones auxiliares para pruebas
// imprime todo el abb de doctores en orden
bool visitar(const char* clave, void* dato, void* extra){
    printf("doctor: %s especialidad: %s \n", clave,((campo_doctores_t*)dato)->especialidad);
    return true;
}

int main(int argc, char** argv){ 	
	if (argc != 3) return 1;
	char* archivo_doctores = argv[POSICION_ARCHIVO_DOCTORES];
	char* archivo_pacientes = argv[POSICION_ARCHIVO_PACIENTES];
	estructuras_t* estructura = leo_archivos(archivo_doctores, archivo_pacientes);
	if (!estructura) return 1;
	// imprime todo el abb de doctores 
	abb_in_order(estructura->doctores,visitar,NULL);

	// imprime todo el hash de pacientes
	hash_iter_t* iter = hash_iter_crear(estructura->pacientes);
	while (!hash_iter_al_final(iter)){
		const char* clave = hash_iter_ver_actual(iter);
		printf("paciente: %s anio: %d \n",clave,*(int*)hash_obtener(estructura->pacientes,clave));
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	

	destruir_estructura(estructura);		
	return 0;
}












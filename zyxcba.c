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
void* abortar_ejecucion(char* linea, char** campos, FILE* archivo, clinica_t* clinica){
	free(linea);
	free_strv(campos);
	fclose(archivo);               // hay que hacer una funcion auxiliar que ejecute estas 4 lineas
	destruir_clinica(clinica); // quizas meter todo aca
	return NULL;
}

clinica_t* leo_archivos(char* archivo_doctores, char* archivo_pacientes){
	 
	FILE*  archivo = fopen(archivo_doctores, "r");
	if(!archivo) return NULL;

	clinica_t* clinica = clinica_crear();
	if(!clinica){
		fclose(archivo);
		return NULL;
	}
	
	char* linea = NULL;
	size_t capacidad = 0;
	
	while(getline(&linea, &capacidad, archivo) > 0){
		char** campos = split(linea, ',');
		if(!campos){
			return abortar_ejecucion(linea, campos, archivo, clinica);
		}
		eliminar_fin_linea(campos[1]);
		campo_doctores_t* campo_doctores = campo_doctores_crear(campos[1]);
		if(!campo_doctores){
			return abortar_ejecucion(linea, campos, archivo, clinica);
		}
		if(agregar_doctor(clinica, campos[0], campo_doctores)){
			destruir_campo_doctores(campo_doctores);
			return abortar_ejecucion(linea, campos, archivo, clinica);

		}
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);

	FILE*  archivo2 = fopen(archivo_pacientes, "r");
	if(!archivo){
		destruir_clinica(clinica);
		return NULL;
	}
	linea = NULL;
	capacidad = 0;
	
	while(getline(&linea, &capacidad, archivo2) > 0){
		char** campos = split(linea, ',');
		if(!campos || !es_numero(campos[1])){ // sin todas esas lineas se pierde memoria
			return abortar_ejecucion(linea, campos, archivo2, clinica);
		}
		if(!agregar_paciente(clinica, campos[0], campos[1])){
			return abortar_ejecucion(linea, campos, archivo2, clinica);
		}
		free_strv(campos);
	}
	fclose(archivo2);
	free(linea);
	return clinica;
}

int main(int argc, char** argv){ 	
	if (argc != 3) return 1;
	char* archivo_doctores = argv[POSICION_ARCHIVO_DOCTORES];
	char* archivo_pacientes = argv[POSICION_ARCHIVO_PACIENTES];
	clinica_t* clinica = leo_archivos(archivo_doctores, archivo_pacientes);
	if (!clinica) return 1;
	destruir_clinica(clinica);		
	return 0;
}












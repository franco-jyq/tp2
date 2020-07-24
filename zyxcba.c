#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include <stdbool.h>
#include <stdlib.h>
#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define POSICION_ARCHIVO_DOCTORES 1
#define POSICION_ARCHIVO_PACIENTES 2
#define URGENCIA "URGENTE"
#define REGULAR "REGULAR"

bool verificar_validez_turno(clinica_t* clinica,const char* paciente,const char* especialidad,const char* urgencia);
void imprimir_doctores(lista_t* lista, clinica_t* clinica);
void procesar_comando(const char* comando, char** parametros, clinica_t* clinica);
void procesar_entrada(clinica_t* clinica);
void* abortar_ejecucion(char* linea, char** campos, FILE* archivo, clinica_t* clinica);
clinica_t* leo_archivos(char* archivo_doctores, char* archivo_pacientes);
bool verificar_parametros (const char* comando,char* parametro1,char* parametro2,char* parametro3); 



void procesar_comando(const char* comando, char** parametros, clinica_t* clinica) {	
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (!verificar_parametros(comando,parametros[0],parametros[1],parametros[2])) return;
		if (!verificar_validez_turno(clinica,parametros[0],parametros[1],parametros[2])) return;
		if (strcmp(parametros[2],URGENCIA) == 0) {
			if (!sacar_turno_urgente(clinica,parametros[0],parametros[1])) return;
		}
		if (strcmp(parametros[2],REGULAR) == 0) {
			if (!sacar_turno_regular(clinica,parametros[0],parametros[1])) return;
		}
		printf(PACIENTE_ENCOLADO,parametros[0]);
		printf(CANT_PACIENTES_ENCOLADOS,cantidad_pacientes_especialidad(clinica,parametros[1]),parametros[1]);			
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!verificar_parametros(comando,parametros[0],NULL,NULL)) return;
		char* paciente = NULL;
		char* especialidad = NULL;
		if (atender_siguiente(clinica, parametros[0], &paciente, &especialidad)) {			
			if (paciente != NULL) {
				printf(PACIENTE_ATENDIDO, paciente);
				printf(CANT_PACIENTES_ENCOLADOS, cantidad_pacientes_especialidad(clinica, especialidad), especialidad);
				free(paciente);
			}	
		} else if (especialidad != NULL && cantidad_pacientes_especialidad(clinica,especialidad) == 0) printf(SIN_PACIENTES);
		else (printf(ENOENT_DOCTOR, parametros[0]));

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if (!verificar_parametros(comando,parametros[0],parametros[1],NULL)) return;
		else {
			lista_t* listado_doctores = generar_informe(clinica, parametros[0], parametros[1]);
			imprimir_doctores(listado_doctores, clinica);
		}
	} else {
		printf(ENOENT_CMD, comando);
	}
}

bool verificar_parametros (const char* comando,char* parametro1,char* parametro2,char* parametro3){
	if (strcmp(comando,COMANDO_PEDIR_TURNO) == 0 && (!parametro1 || !parametro2 || !parametro3)){
		printf(ENOENT_PARAMS,COMANDO_PEDIR_TURNO);
		return false;
	}
	if (strcmp(comando,COMANDO_ATENDER) == 0 && !parametro1) {
		printf(ENOENT_PARAMS,COMANDO_ATENDER);
		return false;
	}
	if (strcmp(comando,COMANDO_INFORME) == 0 && (!parametro1 || !parametro2)){
		printf(ENOENT_PARAMS,COMANDO_INFORME);
		return false;
	}
	return true;
}

void imprimir_doctores(lista_t* lista, clinica_t* clinica){
	if (!lista) return;
	printf(DOCTORES_SISTEMA, lista_largo(lista));
	size_t cont = 1;
	while (!lista_esta_vacia(lista)) {
		char* doctor = lista_borrar_primero(lista);
		char* especialidad = obtener_especialidad(clinica, doctor);
		size_t atendidos = obtener_atendidos(clinica, doctor);
		printf(INFORME_DOCTOR, cont, doctor, especialidad, atendidos);
		cont++;
	}
	lista_destruir(lista, NULL);
}


bool verificar_validez_turno(clinica_t* clinica,const char* paciente,const char* especialidad,const char* urgencia){
	if (!paciente_pertenece (clinica,paciente)) {
			printf(ENOENT_PACIENTE,paciente);
			return false;
		}
	if (!especialidad_pertence(clinica,especialidad)) {
			printf(ENOENT_ESPECIALIDAD,especialidad);
			return false;
		}
	if (strcmp(urgencia,URGENCIA) != 0 && strcmp(urgencia,REGULAR) != 0) {
			printf(ENOENT_URGENCIA,urgencia);
			return false; 
		}
	return true;		
}

void procesar_entrada(clinica_t* clinica) {
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
		procesar_comando(campos[0], parametros, clinica);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}


void* abortar_ejecucion(char* linea, char** campos, FILE* archivo, clinica_t* clinica){
	free(linea);
	free_strv(campos);
	fclose(archivo);               
	destruir_clinica(clinica); 
	return NULL;
}

clinica_t* leo_archivos(char* archivo_doctores, char* archivo_pacientes){	 
	FILE*  archivo = fopen(archivo_doctores, "r");
	if (!archivo) {
		printf(ENOENT_ARCHIVO,archivo_doctores);
		return NULL;
	} 
	clinica_t* clinica = clinica_crear();
	if (!clinica) {
		fclose(archivo);
		return NULL;
	}	
	char* linea = NULL;
	size_t capacidad = 0;
	
	while (getline(&linea, &capacidad, archivo) > 0) {
		char** campos = split(linea, ',');
		if (!campos) {
			return abortar_ejecucion(linea, campos, archivo, clinica);
		}
		eliminar_fin_linea(campos[1]);
		campo_doctores_t* campo_doctores = campo_doctores_crear(campos[1]);
		if (!campo_doctores) {
			return abortar_ejecucion(linea, campos, archivo, clinica);
		}

		if (!agregar_doctor(clinica, campos[0], campo_doctores)) {
			destruir_campo_doctores(campo_doctores);
			return abortar_ejecucion(linea, campos, archivo, clinica);
		}
		if (!agregar_especialidad(clinica,campos[1])) {
			return abortar_ejecucion(linea, campos, archivo, clinica);
		}
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	
	FILE*  archivo2 = fopen(archivo_pacientes, "r");
	if (!archivo) {
		printf(ENOENT_ARCHIVO,archivo_pacientes);
		destruir_clinica(clinica);
		return NULL;
	}
	linea = NULL;
	capacidad = 0;
	
	while (getline(&linea, &capacidad, archivo2) > 0) {
		char** campos = split(linea, ',');
		eliminar_fin_linea(campos[1]);
		if (!campos || !es_numero(campos[1])) {
			printf(ENOENT_ANIO,campos[1]);
			return abortar_ejecucion(linea, campos, archivo2, clinica);
		}
		if (!agregar_paciente(clinica, campos[0], campos[1])) {
			return abortar_ejecucion(linea, campos, archivo2, clinica);
		}
		free_strv(campos);
	}
	fclose(archivo2);
	free(linea);
	return clinica;
}

int main(int argc, char** argv){ 	
	if (argc != 3) {
		printf(ENOENT_CANT_PARAMS);
		return 1;
	} 
	char* archivo_doctores = argv[POSICION_ARCHIVO_DOCTORES];
	char* archivo_pacientes = argv[POSICION_ARCHIVO_PACIENTES];
	clinica_t* clinica = leo_archivos(archivo_doctores, archivo_pacientes);
	if (!clinica) return 1;
	procesar_entrada(clinica);
	destruir_clinica(clinica);		
	return 0;
}












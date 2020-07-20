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


/*Struct de clinicas*/

struct clinica{
	abb_t* doctores;
	hash_t* pacientes;
	hash_t* colas_de_urgencia;
	hash_t* colas_regulares;
};

void destruir_clinica(clinica_t* clinica){	
	if(clinica->pacientes) hash_destruir(clinica->pacientes);
	if(clinica->doctores) abb_destruir(clinica->doctores);
	if(clinica->colas_de_urgencia) hash_destruir(clinica->colas_de_urgencia);
	if(clinica->colas_regulares) hash_destruir(clinica->colas_regulares);
	free(clinica);
}

clinica_t* clinica_crear(){
	clinica_t* clinica = malloc(sizeof(clinica_t));
	if(!clinica) return NULL;	
	
	clinica->doctores = abb_crear(strcmp, destruir_campo_doctores); 
	if(!clinica->doctores){
		destruir_clinica(clinica);
		return NULL;
	}
	
	clinica->pacientes = hash_crear(free);
	if(!clinica->pacientes){
		destruir_clinica(clinica);
		return NULL;
	}
	clinica->colas_de_urgencia = hash_crear(free);
	if(!clinica->colas_de_urgencia){
		destruir_clinica(clinica);
		return NULL;
	}

	clinica->colas_regulares = hash_crear(free);
	if(!clinica->colas_regulares){
		destruir_clinica(clinica);
		return NULL;
	}

	return clinica;
}


bool agregar_doctor(clinica_t* clinica, char* doctor, void* campo_doctores){
	return abb_guardar(clinica->doctores, doctor, campo_doctores);
}
 
bool agregar_paciente(clinica_t* clinica, char* paciente, char* anio){
	int* numero = malloc(sizeof(int));
	if(!numero) return false;
	*numero = atoi(anio);
    if(!hash_guardar(clinica->pacientes, paciente, numero)){	
			free(numero);
			return false;
	}
    return true;

}

bool atender_siguiente(clinica_t* clinica, char* doctor){
	campo_doctores_t* campo = abb_obtener(clinica->doctores, doctor);
	if(!campo){
		printf(ENOENT_DOCTOR, doctor);
		return true;
	}
	if(!lista_esta_vacia(hash_obtener(clinica->colas_de_urgencia, campo->especialidad))){
		char* paciente = lista_borrar_primero(hash_obtener(clinica->colas_de_urgencia, campo->especialidad));
		printf(PACIENTE_ATENDIDO, paciente);
		printf(CANT_PACIENTES_ENCOLADOS, lista_largo(clinica->colas_de_urgencia, campo->especialidad));
	}
	else if(!heap_esta_vacio(hash_obtener(clinica->colas_regulares, campo->especialidad))){
		char* paciente = heap_desencolar(hash_obtener(clinica->colas_regulares, campo->especialidad));
		printf(PACIENTE_ATENDIDO, paciente);
		printf(CANT_PACIENTES_ENCOLADOS, heap_cantidad(hash_obtener(clinica->colas_regulares, campo->especialidad)));
	}
	else{
		printf(SIN_PACIENTES);
	}
	return true;
}

/*Funciones auxiliares*/

bool es_numero(char* str){
	for (int x = 0; str[x] != '\0'; x++){
		if (isdigit(str[x]) == 0) return false;
	}
	return true;
}
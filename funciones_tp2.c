#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include "heap.h"
#include "abb.h"
#include "lista.h"
#include "funciones_tp2.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>


/*Struct campo_paciente */

struct campo_pacientes {
	char* nombre;
	int antiguedad;
};

campo_pacientes_t* campo_pacientes_crear (char* nombre,int antiguedad){
	campo_pacientes_t* campo_pacientes = malloc(sizeof(campo_pacientes_t));
	if (!campo_pacientes) return NULL;
	campo_pacientes->nombre = nombre;
	campo_pacientes->antiguedad = antiguedad;
	return campo_pacientes;
}

void campo_pacientes_destruir (void* campo_pacientes){
	free(((campo_pacientes_t*)campo_pacientes)->nombre);
	free((campo_pacientes_t*)campo_pacientes);
}

/*Struct campo_doctores*/

struct campo_doctores{
	char* especialidad;
	size_t atendidos;
};

campo_doctores_t* campo_doctores_crear(char* especialidad){
	campo_doctores_t* campo_doctores = malloc(sizeof(campo_doctores_t));
	if (!campo_doctores) return NULL;
	campo_doctores->especialidad = strdup(especialidad);
	if (!campo_doctores->especialidad) {
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
	hash_t* especialidades;
	hash_t* colas_de_urgencia;
	hash_t* colas_regulares;
};

void destruir_clinica(clinica_t* clinica){	
	if (clinica->pacientes) hash_destruir(clinica->pacientes);
	if (clinica->doctores) abb_destruir(clinica->doctores);
	if (clinica->colas_de_urgencia) {
		hash_iter_t* iter = hash_iter_crear(clinica->colas_de_urgencia);
		while (!hash_iter_al_final(iter)) {
			const char* actual = hash_iter_ver_actual(iter);
			lista_t* lista = hash_obtener(clinica->colas_de_urgencia,actual);
			lista_destruir(lista,free);
			hash_iter_avanzar(iter);
		}
		hash_iter_destruir(iter);
		hash_destruir(clinica->colas_de_urgencia);
	}
	
	if (clinica->colas_regulares) {
		hash_iter_t* iter = hash_iter_crear(clinica->colas_regulares);
		while (!hash_iter_al_final(iter)) {
			const char* actual = hash_iter_ver_actual(iter);
			heap_t* heap = hash_obtener(clinica->colas_regulares,actual);
			heap_destruir(heap,campo_pacientes_destruir);
			hash_iter_avanzar(iter);
		}
		hash_iter_destruir(iter);
		hash_destruir(clinica->colas_regulares);
	}
	if (clinica->especialidades) hash_destruir(clinica->especialidades);
	free(clinica);
}

clinica_t* clinica_crear(){
	clinica_t* clinica = malloc(sizeof(clinica_t));
	if (!clinica) return NULL;	
	
	clinica->doctores = abb_crear(strcmp, destruir_campo_doctores); 
	if (!clinica->doctores) {
		destruir_clinica(clinica);
		return NULL;
	}
	
	clinica->pacientes = hash_crear(free);
	if (!clinica->pacientes) {
		destruir_clinica(clinica);
		return NULL;
	}

	clinica->especialidades = hash_crear(NULL);
	if (!clinica->especialidades) {
		destruir_clinica(clinica);
		return NULL;
	}

	clinica->colas_de_urgencia = hash_crear(NULL);
	if (!clinica->colas_de_urgencia) {
		destruir_clinica(clinica);
		return NULL;
	}

	clinica->colas_regulares = hash_crear(NULL);
	if (!clinica->colas_regulares) {
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
	if (!numero) return false;
	*numero = atoi(anio);
    if (!hash_guardar(clinica->pacientes, paciente, numero)) {	
			free(numero);
			return false;
	}
    return true;
}

bool agregar_especialidad (clinica_t* clinica,const char* especialidad){
	return hash_guardar(clinica->especialidades,especialidad,NULL);
}

bool paciente_pertenece (clinica_t* clinica,const char* paciente){
	return hash_pertenece(clinica->pacientes,paciente);
}

bool especialidad_pertence (clinica_t* clinica,const char* especialidad){
	return hash_pertenece(clinica->especialidades,especialidad);
}

size_t cantidad_pacientes_especialidad(clinica_t* clinica,const char* especialidad){
	size_t largo1,largo2;
	lista_t* lista = (hash_obtener(clinica->colas_de_urgencia,especialidad));
	if (!lista) largo1 = 0;
	else {
		largo1 = lista_largo(lista);
	}		
	heap_t* heap = hash_obtener(clinica->colas_regulares,especialidad);
	if (!heap) largo2 = 0;
	else {
		largo2 = heap_cantidad(heap);
	}	
	return largo1 + largo2;
}

bool sacar_turno_urgente (clinica_t* clinica,const char* paciente,const char* especialidad){
	if (!hash_pertenece(clinica->colas_de_urgencia,especialidad)) {
		lista_t* lista = lista_crear();
		if (!lista) return false;
		if (!hash_guardar(clinica->colas_de_urgencia,especialidad,lista)) {
			lista_destruir(lista,NULL);
			return false;
		}
	}
	lista_t* lista = hash_obtener(clinica->colas_de_urgencia,especialidad);
	if (!lista) return false;
	char* copia_paciente = strdup(paciente);
	if (!copia_paciente) return false;
	if (!lista_insertar_ultimo(lista,copia_paciente)) return false;
	return true;
}

bool sacar_turno_regular (clinica_t* clinica,const char* paciente,const char* especialidad){
	if (!hash_pertenece(clinica->colas_regulares,especialidad)) {
		heap_t* heap = heap_crear(cmp_pacientes);
		if (!heap) return false;
		if (!hash_guardar(clinica->colas_regulares,especialidad,heap)) {
			heap_destruir(heap,NULL);
			return false;
		}
	}
	heap_t* heap = hash_obtener(clinica->colas_regulares,especialidad);
	if (!heap) return false;
	char* nombre = strdup(paciente);
	if (!nombre) return false;
	campo_pacientes_t* campo_paciente = campo_pacientes_crear(nombre,*(int*)hash_obtener(clinica->pacientes,nombre)); 
	if (!heap_encolar(heap,campo_paciente)) return false;
	return true;
}

bool verificar_urgencias(clinica_t* clinica, char* especialidad, char** copia_paciente_urgente){
	lista_t* lista = hash_obtener(clinica->colas_de_urgencia, especialidad);
	if (!lista || lista_esta_vacia(lista)) return false;
	char* paciente_urgente = lista_borrar_primero(lista);
	*copia_paciente_urgente = strdup(paciente_urgente);
	free(paciente_urgente);
	return true;
}

bool verificar_regulares(clinica_t* clinica, char* especialidad, char** copia_paciente_regular){
	heap_t* heap = hash_obtener(clinica->colas_regulares, especialidad);
	if (!heap || heap_esta_vacio(heap)) return false;
	campo_pacientes_t* campo_paciente = heap_desencolar(heap);
	*copia_paciente_regular = strdup(campo_paciente->nombre);
	campo_pacientes_destruir(campo_paciente);
	return true;
}

bool atender_siguiente(clinica_t* clinica, const char* doctor, char** paciente, char** especialidad){
	campo_doctores_t* campo_doctor = abb_obtener(clinica->doctores, doctor);
	if (!campo_doctor)return false;
	*especialidad = campo_doctor->especialidad;
	if (!verificar_urgencias(clinica, *especialidad, paciente)) {
		if (!verificar_regulares(clinica, *especialidad, paciente)) return false;
	}
	campo_doctor->atendidos ++;
	return true;
}

int cmp_pacientes(const void* paciente1,const void* paciente2){
	if (((campo_pacientes_t*)paciente1)->antiguedad < ((campo_pacientes_t*)paciente2)->antiguedad) return 1;
	if (((campo_pacientes_t*)paciente1)->antiguedad > ((campo_pacientes_t*)paciente2)->antiguedad) return -1;
	return 0;
}


lista_t* generar_informe(clinica_t* clinica, char* inicio, char* fin){
	return abb_in_order_por_rango(clinica->doctores, inicio, fin);	
}

char* obtener_especialidad(clinica_t* clinica, char* doctor){
	campo_doctores_t* campo = abb_obtener(clinica->doctores, doctor);
	return campo->especialidad;
}


size_t obtener_atendidos(clinica_t* clinica, char* doctor){
	campo_doctores_t* campo = abb_obtener(clinica->doctores, doctor);
	return campo->atendidos;
}

/*Funciones auxiliares*/

bool es_numero(char* str){
	for (int x = 0; str[x] != '\0'; x++){
		if (isdigit(str[x]) == 0 && str[x] != ' ') return false;
	}
	return true;
}


void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}
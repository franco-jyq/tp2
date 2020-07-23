#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdbool.h>
#include <stddef.h>


/*Declaracion de clinica y estructuras auxiliares*/

typedef struct clinica clinica_t;
typedef struct campo_doctores campo_doctores_t;
typedef struct campo_pacientes campo_pacientes_t;

/*******************************************
* 
*     Funciones del struct campo doctores
*
********************************************/

/*
Crea una estructura que sera el campo del abb donde se almacenan los doctores
Pre: La cadena pasada por parametro es valida
Post: Se creo el campo, se devolvio NULL en caso de error
*/
campo_doctores_t* campo_doctores_crear(char* campo_doctores);

/*
Destruye el campo del abb de los doctores
Pre: El campo fue creado
Post: Se destruyo el campo
*/
void destruir_campo_doctores(void* campo_doctores);

/*******************************************
* 
*     Funciones del struct campo pacientes
*
********************************************/
/*
Crea una estructura que sera el campo del hash con heaps donde se encolen pacientes de urgencia regular
Pre: La cadena pasada por parametro es valida, el entero tambien
Post: Se creo el campo, se devolvio NULL en caso de error 
*/
campo_pacientes_t* campo_pacientes_crear (char* nombre,int antiguedad);
/*
Destruye el campo del hash con heaps de pacientes
Pre: El campo fue creado
Post: Se destruyo el campo
*/
void campo_pacientes_destruir (void* campo_pacientes);

/*******************************************
* 
*     Funciones del struct clinica
*
********************************************/

/*
Crea una clinica que internamente posee las clinica necesarias para almacenar
los datos pasados al programa. Devuelve NULL en caso de error 
Post: Se creo la clinica, se devolvio NULL en caso de error 
*/
clinica_t* clinica_crear();

/*
Destruye la clinica
Pre: La clinica fue creada
Post: Se destruyo la clinica
*/
void destruir_clinica(clinica_t*);


/*
Agrega un doctor al abb de doctores, devuelve false en caso de error
Pre: La clinica fue creada
Post: Se agrego el doctor al abb, se devolvio false en caso de error
*/
bool agregar_doctor(clinica_t* clinica, char* doctor, void* campo_doctores);

/*
Agrega un paciente al hash de pacientes, devuelve false en caso de error
Pre: La clinica fue creada
Post: Se agrego el paciente al hash, se devolvio false en caso de error
*/
bool agregar_paciente(clinica_t* clinica, char* paciente, char* anio);
 
/*
Agrega una especialidad al hash de especialidades, devuelve false en caso de error
Pre: La clinica fue creada
Post: Se agrego la especialidad al hash, se devolvio false en caso de error
*/
bool agregar_especialidad (clinica_t* clinica,const char* especialidad);

/*
Verifica que un paciente este en el hash de pacientes, devuelve false en caso de que no este
Pre: La clinica fue creada
Post: Se devolvio true si el paciente esta en el hash, se devolvio false en caso caso contrario
*/
bool paciente_pertenece (clinica_t* clinica,const char* paciente);

/*
Verifica que una especialidad este en el hash de esepcialidades, devuelve false en caso de que no este
Pre: La clinica fue creada
Post: Se devolvio true si la especialidad esta en el hash, se devolvio false en caso caso contrario
*/
bool especialidad_pertence (clinica_t* clinica,const char* especialidad);

/*
Devuelve la cantidad de pacientes encolados para una especialidad
Pre: La clinica fue creada
Post: Se devolvio la cantidad de pacientes
*/
size_t cantidad_pacientes_especialidad(clinica_t* clinica,const char* especialidad);

bool sacar_turno_urgente (clinica_t* clinica,const char* paciente,const char* especialidad);

bool sacar_turno_regular (clinica_t* clinica,const char* paciente,const char* especialidad);

/*Atiende si es que hay al siguiente paciente*/
bool atender_siguiente(clinica_t* clinica,const char* doctor, char** paciente, char** especialidad);

lista_t* generar_informe(clinica_t* clinica, char* inicio, char* fin);

/**/
char* obtener_especialidad(clinica_t* clinica, char* doctor);

/**/
size_t obtener_atendidos(clinica_t* clinica, char* doctor);
/*******************************************
*
*           Funciones auxiliares
*
********************************************/

/*
Recibe una cadena y determina si representa a un numero entero
Post: Se devolvio true si representa a un numero entero, false en caso contrario
*/
bool es_numero(char* str);

/*
Funcion de comparacion de los heaps de pacientes. Devuelve 1 si la prioridad 
del primero es mayor, -1 si es menor o 0 si es la misma.
Pre: Los campos de pacientes son validos
Post : Se devolvio el entero correspondiente al resultado
*/
int cmp_pacientes(const void* paciente1,const void* paciente2);

#endif 


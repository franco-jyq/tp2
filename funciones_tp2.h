#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdbool.h>
#include <stddef.h>


/*Declaracion de clinica*/

typedef struct clinica clinica_t;
typedef struct campo_doctores campo_doctores_t;

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


/*******************************************
* 
*     Funciones del struct campo doctores
*
********************************************/

/*
Crea una clinica que sera el campo del abb donde se almacenan los doctores
Pre: La cadena pasada por parametro es valida
Post: Se creo la clinica, se devolvio NULL en caso de error 
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
*           Funciones auxiliares
*
********************************************/


/*Agrega un doctor a la base de datos*/
bool agregar_doctor(clinica_t* clinica, char* doctor, void* campo_doctores);

/*Agrega un paciente a la base de datos*/
bool agregar_paciente(clinica_t* clinica, char* paciente, char* anio);
 
/*Atiende si es que hay al siguiente paciente*/
bool atender_siguiente(clinica_t* clinica, char* doctor);
/*
Recibe una cadena y determina si representa a un numero entero
Post: Se devolvio true si representa a un numero entero, false en caso contrario
*/
bool es_numero(char* str);



#endif 


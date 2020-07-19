#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdbool.h>
#include <stddef.h>


/*Declaracion de estructuras*/

typedef struct estructuras estructuras_t;
typedef struct campo_doctores campo_doctores_t;

/*******************************************
* 
*     Funciones del struct estructuras
*
********************************************/

/*
Crea una estructura que internamente posee las estructuras necesarias para almacenar
los datos pasados al programa. Devuelve NULL en caso de error 
Post: Se creo la estructura, se devolvio NULL en caso de error 
*/
estructuras_t* estructuras_crear();

/*
Destruye la estructura
Pre: La estructura fue creada
Post: Se destruyo la estructura
*/
void destruir_estructura(estructuras_t*);


/*******************************************
* 
*     Funciones del struct campo doctores
*
********************************************/

/*
Crea una estructura que sera el campo del abb donde se almacenan los doctores
Pre: La cadena pasada por parametro es valida
Post: Se creo la estructura, se devolvio NULL en caso de error 
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

/*
Recibe una cadena y determina si representa a un numero entero
Post: Se devolvio true si representa a un numero entero, false en caso contrario
*/
bool es_numero(char* str);


#endif 


#ifndef FUNCIONES_H
#define FUNCIONES_H

#include<stdbool.h>
#include<stddef.h>


/*estructuras*/

struct estructuras;
struct campo_doctores;

typedef struct estructuras estructuras_t;
typedef struct campo_doctores campo_doctores_t;


/*******************************************
* 
*     Funciones del struct estructuras
*
********************************************/

/*crea una estructura con un abb y un hash*/
estructuras_t* estructuras_crear();

/*destruir estructura*/
void destruir_estructura(estructuras_t*);


/*******************************************
* 
*     Funciones del struct campo doctores
*
********************************************/

/*crea una estructura con referenci a una cadena y un entero*/
campo_doctores_t* campo_doctores_crear(char* campo_doctores);

typedef void (*destruir_campo_doctores)(campo_doctores_t *);
/*destruir campo doctores*/
//void destruir_campo_doctores(campo_doctores_t* campo_doctores);


/*******************************************
*
*           Funciones auxiliares
*
********************************************/

/*recibe una cadena y determina si representa a un numero*/
bool es_numero(char* str);


#endif 


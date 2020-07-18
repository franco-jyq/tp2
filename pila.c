#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_INCIAL 10
#define CONSTANTE_DE_REDIMENSION 2

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
pila_t* pila_crear(void){
	pila_t* pila = malloc(sizeof(pila_t));
	if(pila == NULL){
		return NULL;
	}
	pila->cantidad = 0;
	pila->capacidad = CAPACIDAD_INCIAL;
	pila->datos = malloc(pila->capacidad * sizeof(void*));
	if(pila->datos == NULL){
		free(pila);
		return NULL;
	}
	return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

bool redimensionar(pila_t *pila){
	size_t var = 0;
	if(pila->cantidad == pila->capacidad){
		var = pila->capacidad * CONSTANTE_DE_REDIMENSION;
	}
	if(pila->cantidad * 4 <= pila->capacidad && pila->capacidad > 10){
    	var = pila->capacidad / CONSTANTE_DE_REDIMENSION; 			
	}
	if(var > 0){
		void** datos_nuevo = realloc(pila->datos, var * sizeof(void*));
		if (datos_nuevo == NULL){
			return false;
		}
		pila->datos = datos_nuevo;
		pila->capacidad = var;		
	}
	return true;
}

bool pila_apilar(pila_t *pila, void* valor){
	if(redimensionar(pila) == false){
		return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad += 1;
	return true;
}

void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	return pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
	}
	redimensionar(pila);
	pila->cantidad -= 1;
	return pila->datos[pila->cantidad];
}

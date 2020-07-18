#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

typedef struct nodo_abb {
    struct nodo_abb* izq;
    struct nodo_abb* der;
    char* clave;
    void* dato;
}nodo_abb_t;

nodo_abb_t* nodo_abb_crear(char* clave, char* dato){
    nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
    if (!nodo) return NULL;
    nodo->clave = clave;
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

void nodo_abb_destruir(nodo_abb_t* nodo){
    free(nodo->clave);
    free(nodo);
}

struct abb{
    nodo_abb_t* raiz;
    size_t cant;
    abb_destruir_dato_t destruir;
    abb_comparar_clave_t cmp;
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb)return NULL;
    abb->raiz = NULL;
    abb->cant = 0;
    abb->destruir = destruir_dato;
    abb->cmp = cmp;
    return abb;
}

void buscar_elemento(const abb_t* arbol, nodo_abb_t* actual , const char* clave, nodo_abb_t** padre, nodo_abb_t** elemento){
    if (!actual){
        *elemento = NULL;
        return;
    }
    int integrer = arbol->cmp(clave, actual->clave);
    if (integrer < 0){ 
        *padre = actual;
        buscar_elemento(arbol, actual->izq, clave, padre, elemento);
    }
    if (integrer > 0){ 
        *padre = actual;
        buscar_elemento(arbol, actual->der, clave, padre, elemento);
    }
    if(integrer == 0){
        *elemento = actual;
    }    
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    char* copia_clave = strdup(clave);
    if (!copia_clave) return false;
    nodo_abb_t* nodo = nodo_abb_crear(copia_clave, dato);
    if (!nodo){
        free(copia_clave);
        return false;
    }                          
    if (!arbol->raiz){                               
        arbol->raiz = nodo;
        arbol->cant ++;
        return true;
    }
    nodo_abb_t* padre = NULL;
    nodo_abb_t* elemento = NULL;
    buscar_elemento(arbol, arbol->raiz, clave, &padre, &elemento);
    if (!elemento){
        int integrer = arbol->cmp(padre->clave, clave);
        if (integrer < 0) padre->der = nodo; 
        if (integrer > 0) padre->izq = nodo;     
        arbol->cant ++;
    }
    else{
        if (arbol->destruir) arbol->destruir(elemento->dato);
        elemento->dato = dato;
        nodo_abb_destruir(nodo);
    }
    return true;
}


nodo_abb_t* buscar_reemplazante(nodo_abb_t* actual){
    actual = actual->der;
    while (actual->izq){
        actual = actual->izq;
    }
    return actual;
}

void* abb_borrar(abb_t* arbol, const char *clave){
    nodo_abb_t* padre = NULL;
    nodo_abb_t* elemento = NULL;
    buscar_elemento(arbol, arbol->raiz, clave, &padre, &elemento);
    if (!elemento) return NULL; 
    void* dato = elemento->dato;
    if (elemento->izq && !elemento->der){                   
        
        if (elemento == arbol->raiz){
            arbol->raiz = elemento->izq;    
            nodo_abb_destruir(elemento);
        }    
        else {
            int integrer = arbol->cmp(padre->clave, elemento->clave);
            if (integrer < 0) padre->der = elemento->izq;       
            if (integrer > 0) padre->izq = elemento->izq;         
            nodo_abb_destruir(elemento);
        }
    }
    else if (elemento->der && !elemento->izq){                    
        if (elemento == arbol->raiz){
            arbol->raiz = elemento->der;   
            nodo_abb_destruir(elemento);
        }
        else {
            int integrer = arbol->cmp(padre->clave, elemento->clave);
            if (integrer < 0) padre->der = elemento->der;
            if (integrer > 0) padre->izq = elemento->der;
            nodo_abb_destruir(elemento);
        }
    }        
    else if(elemento->izq && elemento->der){
        nodo_abb_t* reemplazante =  buscar_reemplazante(elemento);     
        char* nueva_clave = strdup(reemplazante->clave);               
        void* valor = abb_borrar(arbol, nueva_clave);                  
        free(elemento->clave); 
        elemento->clave = nueva_clave;                                 
        elemento->dato = valor;                                        
        return dato;                                                   
    }
    else { 
        if (!padre) arbol->raiz = NULL;                                
        else {
            int integrer = arbol->cmp(padre->clave, elemento->clave);
            if (integrer < 0) padre->der = NULL;                       
            if (integrer > 0) padre->izq = NULL;                       
        }
        nodo_abb_destruir(elemento);  
    }
    arbol->cant -- ;
    return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_abb_t* elemento = NULL;
    nodo_abb_t* padre = NULL;
    buscar_elemento(arbol,arbol->raiz ,clave,&padre,&elemento);
    if(elemento) return elemento->dato;
    return NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_abb_t* elemento = NULL;
    nodo_abb_t* padre = NULL;   
    buscar_elemento(arbol, arbol->raiz ,clave, &padre, &elemento);
    return elemento != NULL;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void _abb_destruir(abb_t* arbol,nodo_abb_t* actual){ 
    if (!actual) return;
    _abb_destruir(arbol,actual->izq);
    _abb_destruir(arbol,actual->der);
    if (arbol->destruir) arbol->destruir(actual->dato); 
    nodo_abb_destruir(actual);
}

void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol,arbol->raiz);
    free(arbol);
}

bool _abb_in_order(nodo_abb_t* actual, bool visitar(const char *, void *, void *), void *extra){
    if (!actual) return true;
    if (!_abb_in_order(actual->izq, visitar, extra)) return false;
    if (!visitar(actual->clave, actual->dato, extra)) return false;
    if (!_abb_in_order(actual->der, visitar, extra)) return false;
    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    _abb_in_order(arbol->raiz, visitar, extra);
}

/*************************************************************************************
 * 
 *                                 Iterador Del Abb
 * 
 * ************************************************************************************/

struct abb_iter{
    const abb_t* abb;
    pila_t* pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* abb_iter = malloc(sizeof(abb_iter_t));
    if (!abb_iter) return NULL;
    pila_t* pila = pila_crear();
    if (!pila){
        free(abb_iter);
        return NULL;
    }
    if (arbol->raiz){        
        nodo_abb_t* actual = arbol->raiz;
        while (actual->izq){
            pila_apilar(pila, actual);
            actual = actual->izq;  
        }
        pila_apilar(pila, actual);
    }
    abb_iter->pila = pila;
    abb_iter->abb = arbol;
    return abb_iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if (pila_esta_vacia(iter->pila)) return false;
    nodo_abb_t* dato = pila_desapilar(iter->pila);
    if (dato->der){
        nodo_abb_t* actual = dato->der;
        while (actual->izq){
            pila_apilar(iter->pila, actual);
            actual = actual->izq;
        }         
        pila_apilar(iter->pila, actual);
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    nodo_abb_t* elemento = pila_ver_tope(iter->pila);
    if (!elemento) return NULL;
    return  elemento->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}







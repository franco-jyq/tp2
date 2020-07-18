#include "strutil.h"
#include <string.h>
#include <stdlib.h>
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

char *substr(const char *str, size_t n){
    char* nueva_cadena = calloc(n+1,sizeof(char));
    if (!nueva_cadena){
        return NULL;
    }
    strncpy(nueva_cadena,str,n);
    return nueva_cadena;
}

char **split(const char *str, char sep){
    size_t cant_sep = 0;
    for (size_t i = 0;i <= strlen(str) && sep != '\0';i++){
        if (str[i] == sep){
            cant_sep++;
        }
    }   
    char** sub_segmentos = calloc(cant_sep+2,sizeof(char*));
    if (!sub_segmentos){
        return NULL;
    }
    size_t indice = 0;
    size_t indice_viejo = 0; 
    for (size_t i = 0;i < cant_sep+1;i++){
        while(str[indice] != sep && str[indice] != '\0'){
            indice++;
        }
        char* cadena_actual = calloc(indice-indice_viejo+1,sizeof(char));
        strncpy(cadena_actual,str+indice_viejo,indice-indice_viejo);
        sub_segmentos[i] = cadena_actual;
        indice++;
        indice_viejo = indice; 
    }
    return sub_segmentos;
}

char *join(char **strv, char sep){
    char* cadena = malloc(sizeof(char)*2);
    if (!cadena){
        return NULL;
    }
    if (!strv[0]){
        cadena[0] = '\0';
        return cadena;
    }
    size_t largo_cadena = 0;    
    for (size_t i = 0;strv[i] != NULL;i++){
        size_t largo_viejo = largo_cadena;
        largo_cadena += strlen(strv[i]);
        cadena = realloc(cadena,sizeof(char)*(largo_cadena+1));
        if (!cadena){
            return NULL;
        }
        memcpy(cadena+largo_viejo,strv[i],strlen(strv[i]));    
        if (sep != '\0'){
            cadena[largo_cadena] = sep;
            largo_cadena++;
        }

    }
    if (sep == '\0'){
        cadena[largo_cadena] = '\0';
    } else {
        cadena[largo_cadena-1] = '\0';
    }   
    return cadena;
}

void free_strv(char *strv[]){
    for (size_t i = 0;strv[i] != NULL;i++){
        free(strv[i]);
    }
    free(strv);
}



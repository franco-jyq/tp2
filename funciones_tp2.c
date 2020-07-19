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


/*Funciones auxiliares*/

bool es_numero(char* str){
	for (int x = 0; str[x] != '\0'; x++){
		if (isdigit(str[x]) == 0) return false;
	}
	return true;
}
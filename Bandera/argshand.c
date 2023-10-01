#include <stdio.h>
#include <string.h>
#include "argshand.h"

/* --------------------------------------------------------------------------
 * Funciones optimizadas: No hace falta comprobar si las cadenas tienen la
 * misma longitud (con strlen) y luego si tienen el mismo valor (con strstr).
 *
 * En vez de ello, podemos comparar directaente las cadenas con la funcion
 * strcmp que devuelve 0 si son iguales y 1 en caso contrario
 * ---------------------------------------------------------------------------
 * * * * */

char* GetArg(char const *str_to_find, int narg, char **arg) {
    for(short i=0;i<narg;i++)                
        if (!strcmp(arg[i], str_to_find)) 
            return arg[i+1];          
    return NULL;           
}

int ExistArg(char const *str_to_find, int narg, char **arg) {
    for(short i=0;i<narg;i++)
        if (!strcmp(arg[i], str_to_find)) 
            return 1;
    return 0;
}

#include "misc.h"


#define BLOCK_STR 20

void * reallocMem(void * target, size_t spaces, int * flag){
      void * ptr = realloc(target, spaces);
      if(ptr == NULL){
            free(target);
            *flag = NO_MEMORY;
            return NULL;
      }
      *flag = OK;
      return ptr;
}

char * copyString(char * s, size_t * len, int * flag){
	char * rta = NULL;
	int i;
	for(i=0; s[i] != '\n' && s[i] != '\0'; i++){
		if(i%BLOCK_STR == 0){
			rta = realloc(rta, i+BLOCK_STR);
            if(rta == NULL){
                *flag = NO_MEMORY;
                return NULL;
            }
		}
		rta[i] = s[i];
	}
	rta = realloc(rta, i+1); // No deberia fallar ya que se achica el string. (Checkear con la madre tierra).
    rta[i] = 0;
	*len = i;
	return rta;
}
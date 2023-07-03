#include "errores.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifndef _MISC_H
#define _MISC_H

void * reallocMem(void * target, size_t spaces, int * flag);
char * copyString(char * s, size_t * len, int * flag);

#endif
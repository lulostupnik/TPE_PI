#ifndef query3_h 
#define query3_h 
#include "readDataADT.h"
#include <stddef.h>


typedef struct Query3CDT * query3ADT; 

query3ADT newQuery(readDataADT bikes, readDataADT stations, int *flag, size_t idDepar, size_t dateDepar); 

#endif
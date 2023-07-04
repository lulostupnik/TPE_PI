#ifndef query2_h
#define query2_h
#include "readDataADT.h"
#include <stddef.h>
typedef struct query2CDT * query2ADT;
query2ADT newQuery(readDataADT bikes,readDataADT stations,int * flag,size_t  idDep, size_t idArr);
#endif
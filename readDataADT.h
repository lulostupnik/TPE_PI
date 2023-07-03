#include "errores.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifndef _READEXCELTABLES_H
#define _READEXCELTABLES_H


typedef struct readDataCDT * readDataADT;


size_t getDimRowsExcel(readDataADT data, int * flag);
size_t getDimColsExcel(readDataADT data, int * flag);
char * getDataFromPos(readDataADT data, size_t row, size_t col, int * flag);
readDataADT newRead(char * tablePath, size_t * columnsToCopy , size_t dimColumns, size_t maxLenght, int * flag);
void freeExcelData(readDataADT data);


#endif
#ifndef _READEXCELTABLES_H
#define _READEXCELTABLES_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


typedef enum {OK=0, INCORRECT_PATH, INVALID_POSITION, NO_MEMORY} tStatus;

typedef struct readDataCDT * readDataADT;

#include "readDataADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

size_t getDimRowsExcel(readDataADT data);
size_t getDimColsExcel(readDataADT data);
char * getDataFromPos(readDataADT data, size_t row, size_t col, int * flag);
readDataADT newRead(char * tablePath, size_t * columnsToCopy , size_t dimColumns, size_t maxLenght, int * flag);
struct excelData * getData(FILE * fp,size_t * dimRows, size_t * columnsToCopy, size_t dimVecColumns,size_t maxLenght, int * flag);
void freeExcelData(readDataADT data);


#endif
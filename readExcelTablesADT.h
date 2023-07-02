#ifndef _READEXCELTABLES_H
#define _READEXCELTABLES_H
#include <stddef.h>


#define INCORRECT_PATH 1
#define INVALID_POSITION 3

typedef struct readExcelTablesCDT * readExcelTablesADT;

readExcelTablesADT newRead(char * tablePath, int * flag);
size_t getDimColsExcel(readExcelTablesADT data);
size_t getDimRowsExcel(readExcelTablesADT data);
char * getDataFromPos(readExcelTablesADT data, size_t row, size_t col, int * flag);
void freeExcelData(readExcelTablesADT data);

#endif
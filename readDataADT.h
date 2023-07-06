#include <stddef.h>
#ifndef _READDATAADT_H
#define _READDATAADT_H


typedef enum {ID_START, ID_END, START, MEMBER} colsBikes;
typedef enum {NAME, ID_} colsStation;

typedef struct readDataCDT * readDataADT;
readDataADT newReadstation(char * tablePath, size_t stationNameCol, size_t stationIdCol, size_t maxLenght, int * flag);
readDataADT newReadBikes(char * tablePath, size_t bikeIdDepar, size_t bikeDate,size_t bikeIdArr,size_t isMember, size_t maxLenght, int * flag);
void printStation(readDataADT data); //borrar
void freeExcel(readDataADT data);
size_t getrows(readDataADT data);
size_t getcols(readDataADT data);
void toBegin(readDataADT data);
int hasNext(readDataADT data) ;
char * getDataFromPosBike(readDataADT data, size_t row, colsBikes col,int * flag);

#endif

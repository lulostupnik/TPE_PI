#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "errores.h"
#include "readDataADT.h"
#include "misc.h"

#ifndef _QUERY1ADT_H
#define _QUERY1ADT_H

typedef int (*cmpMember)(char *); // Funcion para saber si un usuario es miembro o no. Retorna 1 si lo es, 0 si no. 

typedef struct query1CDT * query1ADT;

int processDataQ1(query1ADT q1, size_t bikesIdCol, size_t stationIdCol, size_t stationNameCol, size_t isMemberCol, cmpMember cmp);

void printTripsID(query1ADT q1);
void printTripsNAME(query1ADT q1);

void freeQuery1(query1ADT q1);
int addNamesFromVec(query1ADT q1, char ** vec, size_t dim);
int processCantTrips(query1ADT q1, size_t bikesIdCol, size_t isMemberCol, cmpMember cmp);
query1ADT newQuery1(readDataADT bikesExcel, readDataADT stationExcel,int * flag);
int orderByCantTrip(query1ADT q1);

void toBegin(query1ADT query1Adt);
int hasNext(const query1ADT query1Adt);
int next(query1ADT query1Adt, char ** name, size_t * len, size_t * cantTrips);



#endif
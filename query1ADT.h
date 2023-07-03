#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "errores.h"
#include "readDataADT.h"
#include "misc.h"

#ifndef _QUERY1ADT_H
#define _QUERY1ADT_H


typedef struct query1CDT * query1ADT;


void printTrips(query1ADT q1);
int addNamesFromVec(query1ADT q1, char ** vec, size_t dim);
int processDataQ1(query1ADT q1, size_t stationIdCol);
query1ADT newQuery1(readDataADT bikesExcel, readDataADT stationExcel,int * flag);
int addNameList(query1ADT q1, char * name, size_t len, size_t stationId); // checkear cuando haga NYC
int orderByCantTrip(query1ADT q1);

void toBegin(query1ADT query1Adt);
int hasNext(const query1ADT query1Adt);
int next(query1ADT query1Adt, char ** name, size_t * len, size_t * cantTrips);



#endif
#ifndef _QUERY1ADT_H
#define _QUERY1ADT_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct query1CDT * query1ADT;

typedef enum {OK = 0,NULL_POINTER, ENOMEM, INVALID_ID, ALREADY_ADDED} errors;

void printTrips(query1ADT q1);


query1ADT newQuery1(int * flag);
int addTrip(query1ADT q1, size_t stationId);
int addName(query1ADT q1, char * name, size_t len, size_t stationId);
int orderByCantTrip(query1ADT q1);

void toBegin(query1ADT query1Adt);
int hasNext(const query1ADT query1Adt);
int next(query1ADT query1Adt, char ** name, size_t * len, size_t * cantTrips);



#endif
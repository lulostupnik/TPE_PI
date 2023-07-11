#ifndef ADT_h
#define ADT_h
#include <errno.h>
#include <stddef.h>

typedef struct bikeRentingCDT * bikeRentingADT;

bikeRentingADT newBikesRenting();
void freeTad(bikeRentingADT ADT);

/*
** Devuelve 0 si se agrego la estacion, -1 si no se agrego la estacion por falta de memoria (setea errno en enomem), y 1 si no agrego la estacion porque ya habia una con ese ID.
*/
int addStation(char * name,size_t id,bikeRentingADT ADT);

/*
** Devuelve 1 no se proceso la informacion por paramtros invalidos (month, idStart, idEnd)
*/
int processData(bikeRentingADT ADT,int month,int isMember,size_t idStart,size_t idEnd);

size_t getNumberOfStations(bikeRentingADT ADT);






//Funciones Iteradoras Para Q1:
void toBeginQ1(bikeRentingADT ADT);
int hasNextQ1(bikeRentingADT ADT);
char * getNameQ1(bikeRentingADT ADT);
void nextQ1(bikeRentingADT ADT);
size_t getTripsQ1(bikeRentingADT ADT);

//Funciones Iteradoras Para Q2:
int hasNextStartQ2(bikeRentingADT ADT);
void toBeginQ2(bikeRentingADT ADT);
int hasNextDestinationQ2(bikeRentingADT ADT);
size_t getTravelsToQ2(bikeRentingADT ADT);
size_t getTravelsFromQ2(bikeRentingADT ADT);
char * getNameOfDestinationQ2(bikeRentingADT ADT);
char * getNameOfStartQ2(bikeRentingADT ADT); 
void nextDestinationQ2(bikeRentingADT ADT);
void nextStartStationQ2(bikeRentingADT ADT);

//Funciones Iteradoras Para Q3
char * getNameQ3(bikeRentingADT ADT);
int hasNextQ3(bikeRentingADT ADT);
void toBeginQ3(bikeRentingADT ADT);
void nextQ3(bikeRentingADT ADT);
void  getTravelsByMonthQ3(bikeRentingADT ADT,size_t travels[]);
#endif
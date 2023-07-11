#include <stddef.h>
#ifndef ADT_h
#define ADT_h
#include <errno.h>


typedef struct bikeRentingCDT * bikeRentingADT;

bikeRentingADT newBikesRenting();

//Funciones Iteradoras Para Q1:
void toBeginQ1(bikeRentingADT ADT);
int hasNextQ1(bikeRentingADT ADT);
char * getNameQ1(bikeRentingADT ADT);
void nextQ1(bikeRentingADT ADT);
size_t getTripsQ1(bikeRentingADT ADT);

//Funciones Iteradoras Para Q2:
int hasNextQ2(bikeRentingADT ADT);
void toBeginQ2(bikeRentingADT ADT);
int hasNextDestinationQ2(bikeRentingADT ADT);
size_t getTravelsToQ2(bikeRentingADT ADT);
size_t getTravelsFromQ2(bikeRentingADT ADT);
char * getNameOfDestination(bikeRentingADT ADT);
char * startStationName(bikeRentingADT ADT); //NOSE SI ESTA ES DE Q2 EXACTAMENTE 
void nextDestinationQ2(bikeRentingADT ADT);
void nextQ2(bikeRentingADT ADT);

//Funciones Iteradoras Para Q3
char * getNameQ3(bikeRentingADT ADT);
int hasNextQ3(bikeRentingADT ADT);
void toBeginQ3(bikeRentingADT ADT);
void nextQ3(bikeRentingADT ADT);


void addStation(char * name,size_t id,bikeRentingADT ADT);
void processData(bikeRentingADT ADT,int month,int isMember,size_t idStart,size_t idEnd);

size_t getNumberOfStations(bikeRentingADT ADT);


void  getTravelsByMonth(bikeRentingADT ADT,size_t travels[]);
void freeTad(bikeRentingADT ADT);


#endif
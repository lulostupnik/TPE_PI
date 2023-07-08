#include <stddef.h>
#ifndef ADT_h
#define ADT_h

typedef struct bikeRentingCDT * bikeRentingADT;

bikeRentingADT newBikesRenting();

void addStation(char * name,int id,bikeRentingADT stations);
void processData(bikeRentingADT bikes,int month,int isMember,int idStart,int idEnd);
void orderByName(bikeRentingADT stations);
void sortTravels(bikeRentingADT stations);
size_t getNumberOfStations(bikeRentingADT stations);
size_t getTravels(bikeRentingADT stations,int position);
char * getNameByPosition(bikeRentingADT stations,int position);
size_t getTravelsTo(bikeRentingADT stations,int row,int col);
size_t getTravelsFrom(bikeRentingADT stations,int row,int col);
void  getTravelsByMoth(bikeRentingADT stations,int position,size_t travels[]);
void freeTad(bikeRentingADT stations);


#endif
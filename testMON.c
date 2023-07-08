#include "ADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_LENGHT 512
#define StationNameCol 1
#define StationIdcol 0
#define DATE_COL 0
#define ID_START_COL 1
#define ID_END_COL 3
#define ISMEMBER 4
void readDataStations(FILE * dataStations,bikeRentingADT stations);
void readDataBikes( FILE * dataBikes,bikeRentingADT stations);
int loadQuery3 (bikeRentingADT tad);
int loadQuery1 (bikeRentingADT tad);
int loadQuery2 (bikeRentingADT tad);
int main(){
bikeRentingADT stations=newBikesRenting();
FILE * dataStations=fopen("./stationsMON.csv","r");
FILE * dataBikes=fopen("./bikesMON.csv","r");
readDataStations(dataStations,stations);
readDataBikes(dataBikes,stations);
loadQuery1(stations);
loadQuery2(stations);
loadQuery3(stations);
freeTad(stations);
}

void readDataStations(FILE * dataStations,bikeRentingADT stations){
int id;
char delim[]=";\n";
int firstRead=1;
char buffer[BUFFER_LENGHT], * token, * name; 
while((fgets(buffer,BUFFER_LENGHT,dataStations))!=NULL){
    int j=0;
    if(!firstRead){
    token = strtok(buffer, delim);
    while(token !=NULL){
     if(j==StationNameCol){
      name=token;  
     }else if(j==StationIdcol){
     id=atoi(token);
     }
     j++;
     token=strtok(NULL,delim);
    }
     addStation(name,id,stations);
}
firstRead=0;
}

}
void readDataBikes( FILE * dataBikes,bikeRentingADT stations){
int idStart,idEnd,month,isMember;
int firstRead=1;
char buffer[BUFFER_LENGHT], * token;
char delim[]=";\n";
while(fgets(buffer,BUFFER_LENGHT,dataBikes)!=NULL){
    int j=0;
    if(!firstRead){
     token = strtok(buffer, delim);
     while(token !=NULL){
     if(j==DATE_COL){
      sscanf(token,"%*d-%d-%*d",&month);
     }else if(j==ID_START_COL){
     idStart=atoi(token);
     }else if(j==ID_END_COL){
     idEnd=atoi(token);
     } else if(j==ISMEMBER){
     isMember=atoi(token);
     }
     token=strtok(NULL,delim);
     j++;
     }
     processData(stations,month,isMember,idStart,idEnd);
     }
     firstRead=0;
     
}
}

int loadQuery1 (bikeRentingADT tad){
    sortTravels(tad);
    size_t size =getNumberOfStations(tad);
    size_t travels;
    char * name;
     for (int i=0; i < size; i++) {
        travels = getTravels(tad, i);
        name = getNameByPosition(tad, i);
        printf("%-20s\t %-20ld\n",name,travels);
}
}
int loadQuery2 (bikeRentingADT tad){
    orderByName(tad);
    size_t size =getNumberOfStations(tad);
    size_t travelsTo;
    size_t travelsFrom;
    char * nameEnd;
    char * nameStart;
     for (int i=0; i < size; i++) {
        nameStart = getNameByPosition(tad, i);
        for(int j=0;j<size;j++){
        travelsTo = getTravelsTo(tad, i,j);
        travelsFrom=getTravelsFrom(tad, i,j);
        char * nameEnd=getNameByPosition(tad, j);
        if(travelsFrom != 0 || travelsTo !=0){
        printf("%-30s\t %-30s\t %-30ld\t %-30ld\n",nameStart,nameEnd,travelsTo,travelsFrom);
        }
}}
}
int loadQuery3 (bikeRentingADT tad){
    orderByName(tad);
    size_t size =getNumberOfStations(tad);
    size_t travels[12];
    char * name;
     for (int i=0; i < size; i++) {
         getTravelsByMoth(tad, i,travels);
         name = getNameByPosition(tad, i);
         printf("%s\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\n",name,travels[0],travels[1],travels[2],travels[3],travels[4],travels[5],travels[6],travels[7],travels[8],travels[9],travels[10],travels[11]);
}
}
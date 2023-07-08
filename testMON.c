#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "adt.h"



#define BUFFER_LENGHT 512
#define StationNameCol 1
#define StationIdcol 0
#define DATE_COL 0
#define ID_START_COL 1
#define ID_END_COL 3
#define ISMEMBER 4

//typedef enum {STATION_ID_NYC = 3, STATION_NAME_NYC =0,ID_START_NYC =1, ID_END_NYC=3, DATE_NYC=0, ISMEMBER_NYC=5} nycCols;

void readDataStations(FILE * dataStations,bikeRentingADT stations){
int id;
int firstRead=1;
char buffer[BUFFER_LENGHT], * token, * name; 
while((fgets(buffer,BUFFER_LENGHT,dataStations))!=NULL){
    int j=0;
    if(!firstRead){
    token = strtok(buffer, ";");
    while(token !=NULL){
     if(j==StationNameCol){
      name=token;  
     }else if(j==StationIdcol){
     id=atoi(token);
     }
     j++;
     token=strtok(NULL,";");
    }
     addStation(name,id,stations);
}
firstRead=0;
}

}




void readDataBikes( FILE * dataBikes,bikeRentingADT stations){
int idStart,idEnd,month, isMember;
int firstRead=1;
char buffer[BUFFER_LENGHT], * token;// *isMemberString; 

while(fgets(buffer,BUFFER_LENGHT,dataBikes)!=NULL){
    int j=0;
    if(!firstRead){
     token = strtok(buffer, ";");
     while(token !=NULL){
     if(j==DATE_COL){
      sscanf(token,"%*d-%d-%*d",&month);
     }else if(j==ID_START_COL){
     idStart=atoi(token);
     }else if(j==ID_END_COL){
     idEnd=atoi(token);
     } else if(j==ISMEMBER){
    // isMemberString=token;
     isMember = !strcmp(token, "1");
     }
     token=strtok(NULL,";");
     j++;
     }
     processData(stations,month,isMember,idStart,idEnd);
     }
     firstRead=0;    
}
}

int main(){
    FILE * dataStations=fopen("/mnt/c/Users/lulos/Desktop/FINAL PI/EXCELS/stationsMON.csv","r");
    FILE * dataBikes=fopen("/mnt/c/Users/lulos/Desktop/FINAL PI/EXCELS/bikesMON.csv","r");
    bikeRentingADT tad = newTad();
    readDataStations(dataStations,tad);
    
    //printStations(tad);
    //printDebug(tad);
    readDataBikes(dataBikes, tad);
    //printStationsById(tad);
   // printMatrix(tad);
    freeAll(tad);
}



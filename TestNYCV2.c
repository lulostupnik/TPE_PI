#include "ADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htmlTable.h"

#define BUFFER_LENGHT 512
#define StationNameCol 0
#define StationIdcol 3
#define DATE_COL 0
#define ID_START_COL 1
#define ID_END_COL 3
#define ISMEMBER 5
#define TOTAL_QUERIES 3
#define TOTAL_FILES 2 
#define INVALID -1 

void readDataStations(FILE * dataStations,bikeRentingADT TadStations);
void readDataBikes( FILE * dataBikes,bikeRentingADT TadStations);

int loadQuery3 (bikeRentingADT tad, FILE * CSVquery3, htmlTable HTquery3);
int loadQuery1 (bikeRentingADT tad, FILE * CSVquery2, htmlTable HTquery2);
int loadQuery2 (bikeRentingADT tad, FILE * CSVquery1, htmlTable HTquery1);


int main(int argc, char * argv[]){

// VALIDO LOS ARGUMENTOS QUE ME PASAN//
int validArgs = validationArgs(argc, argv); 

    if(validArgs == INVALID){
        return; 
    }

// CREO EL TAD //
bikeRentingADT TadStations = newBikesRenting();

// VERIFICO QUE SE CREO CORRECTAMENTE
    if(TadStations == NULL || errno == ENOMEM){
        //ERROR
    }

// APERTURA DE LOS ARCHIVOS 
FILE * dataStations = fopen("./stationsNYC.csv","r"); //CAMBIAR POR argv[]
FILE * dataBikes = fopen("./bikesNYC.csv","r"); //CAMBIAR POR argv[]

FILE * CSVquery1 = fopen("query1.csv","w"); 
htmlTable HTquery1 = newTable("query.html", 2, "Station", "StartedTrips"); 

FILE * CSVquery2 = fopen("query2.csv", "w"); 
htmlTable HTquery2 = newTable("query2.html", 4, "StationA","StationB","Trips A->B", "Trips B->A" ); 

FILE * CSVquery3 = fopen("query3.csv", "w");
htmlTable HTquery3 = newTable("query3.html", 13, "J","F","M","A","M","J","J","A","S","O","N","D","Station"); 

size_t quantFiles = TOTAL_FILES + TOTAL_QUERIES; 

FILE * fileFolder[] = {dataStations, dataBikes, CSVquery1,CSVquery2, CSVquery3}; 

// CHEQUEAR QUE LOS ARCHIVOS SE HAYAN PASADO DE MANERA CORRECTA (POR UN LADO LOS * FILES Y POR OTRO LOS HTML ? ) 

// CARGO LA DATA DE STATIONS Y DE BIKES EN EL ADT // 
readDataStations(dataStations,TadStations);
readDataBikes(dataBikes,TadStations);

// VALIDAR DATA CARGADA EN EL ADT // 

// CARGO LOS QUERIES 

loadQuery1(TadStations, CSVquery1, HTquery1);
loadQuery2(TadStations, CSVquery2, HTquery2);
loadQuery3(TadStations, CSVquery3, HTquery3);

freeTad(TadStations);
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
     if(strcmp(token,"member")==0){
        isMember=1;
     }else{
     isMember=0;
     }}
     token=strtok(NULL,delim);
     j++;
     }
     processData(stations,month,isMember,idStart,idEnd);
     }
     firstRead=0;
     
}


}

int loadQuery1 (bikeRentingADT tad, FILE * CSVquery1, htmlTable HTquery1){
    sortTravels(tad);
    size_t size = getNumberOfStations(tad);
    size_t travels;
    char * name;

    fprintf(CSVquery1, "Station;StartedTrips\n"); //Titulos Columnas CSV

     for (int i=0; i < size; i++) {
        travels = getTravels(tad, i);
        name = getNameByPosition(tad, i);

        addHTMLRow(HTquery1, name, travels); //Voy Imprimiendo el HTML
    int query1Ans = fprintf(CSVquery1, "%s;%zu\n",name, travels); //Voy Imprimiendo el CSV
        if(query1Ans < 0){
            //ERROR 
        }

        //FUNCIONO
}
closeHTMLTable(HTquery1);
fclose(CSVquery1);
}

int loadQuery2 (bikeRentingADT tad, FILE * CSVquery2, htmlTable HTquery2){

    orderByName(tad);
    size_t size = getNumberOfStations(tad);
    size_t travelsTo;
    size_t travelsFrom;
    char * nameEnd;
    char * nameStart;
    fprintf(CSVquery2, "StationA;StationB;Trips A->B;Trips B->A"); //Titulos Columnas CSV

     for (int i=0; i < size; i++) {
        nameStart = getNameByPosition(tad, i);

        for(int j=0; j<size;j++){
            travelsTo = getTravelsTo(tad, i,j);
            travelsFrom = getTravelsFrom(tad,i,j);
            char * nameEnd = getNameByPosition(tad, j);

            if(travelsFrom != 0 || travelsTo !=0){
                addHTMLRow(HTquery2, nameStart, nameEnd, travelsTo, travelsFrom); //Voy imprimiendo el HTML (chequear orden de parametros)
            int query2Ans = fprintf(CSVquery2, "%s;%s;%zu;%zu\n",nameStart,nameEnd, travelsTo, travelsFrom); //Voy imprimiend el CSV
                if(query2Ans < 0){
                    //ERROR
                }

        }

}
}
//FUNCIONO 
closeHTMLTable(HTquery2);
fclose(CSVquery2);
}

int loadQuery3 (bikeRentingADT tad, FILE * CSVquery3, htmlTable HTquery3){
    orderByName(tad);
    size_t size =getNumberOfStations(tad);
    size_t travels[12];
    char * name;
     for (int i=0; i < size; i++) {
         getTravelsByMoth(tad, i,travels);
         name = getNameByPosition(tad, i);
         addHTMLRow(HTquery3,travels[0],travels[1],travels[2],travels[3],travels[4],travels[5],travels[6],travels[7],travels[8],travels[9],travels[10],travels[11],name); //Voy imprimiendo el HTML
         int query3Ans = fprintf(CSVquery3, "%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%s\n",travels[0],travels[1],travels[2],travels[3],travels[4],travels[5],travels[6],travels[7],travels[8],travels[9],travels[10],travels[11],name); //Voy imprimiendo el CSV
            if(query3Ans < 0){
                //Error
            }
}
closeHTMLTable(HTquery3);
fclose(CSVquery3);
//FUNCIONO
}
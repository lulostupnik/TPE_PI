#include "ADT.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htmlTable.h"

#define BUFFER_LENGHT 512
#define StationNameCol 1
#define StationIdcol 0
#define DATE_COL 0
#define ID_START_COL 1
#define ID_END_COL 3
#define ISMEMBER 4
#define TOTAL_QUERIES 3
#define TOTAL_FILES 2 
#define INVALID -1 
#define MONTHS 12 

typedef enum {OK=0,WRONG_AMOUNT_OF_ARGUMENTS,WRONG_PATH,NOT_CREATED,NO_MEM,PRINT_ERROR} mainErrors;

int readDataStations(FILE * dataStations,bikeRentingADT TadStations);
int readDataBikes( FILE * dataBikes,bikeRentingADT TadStations);
void handleErrors(bikeRentingADT tad,mainErrors exitCode,char * error);
int loadQuery3 (bikeRentingADT tad, FILE * CSVquery3, htmlTable HTquery3);
int loadQuery1 (bikeRentingADT tad, FILE * CSVquery2, htmlTable HTquery2);
int loadQuery2 (bikeRentingADT tad, FILE * CSVquery1, htmlTable HTquery1);


int main(int argc, char * argv[]){
    errno = 0;
    int exitCode;
// VALIDO LOS ARGUMENTOS QUE ME PASAN//
    if(argc != 2){
         exitCode=WRONG_AMOUNT_OF_ARGUMENTS; 
         handleErrors(exitCode,"Cantidad erronea de argumentos",NULL);
    }

// CREO EL TAD //
bikeRentingADT TAD = newBikesRenting();

// VERIFICO QUE SE CREO CORRECTAMENTE
    if(TAD == NULL || errno == ENOMEM){
        exitCode=NO_MEM;
        handleErrors(exitCode,"Error al crear el ADT",TAD);
    }

// APERTURA DE LOS ARCHIVOS 
    FILE * dataStations = fopen(argv[1],"r"); 
        if(dataStations == NULL){
            exitCode=WRONG_PATH;
            handleErrors(exitCode,"Archivo no encontrado",TAD);
        }
    FILE * dataBikes = fopen(argv[0],"r"); 
        if(dataBikes == NULL){
            exitCode=WRONG_PATH;
            handleErrors(exitCode,"Archivo no encontrado",TAD);
    }
    FILE * CSVquery1 = fopen("query1.csv","w"); 
        if(CSVquery1 == NULL){
            exitCode=NOT_CREATED;
            handleErrors(exitCode,"No se pudo crear el archivo",TAD);
        }
    htmlTable HTquery1 = newTable("query1.html", 2, "Station", "StartedTrips"); 
        if(HTquery1 == NULL){
            exitCode=NOT_CREATED;
            handleErrors(exitCode,"No se pudo crear el archivo",TAD);
        }

    FILE * CSVquery2 = fopen("query2.csv", "w"); 
        if(CSVquery2 == NULL){
            exitCode=NOT_CREATED;
            handleErrors(exitCode,"No se pudo crear el archivo",TAD);
        }
    htmlTable HTquery2 = newTable("query2.html", 4, "StationA","StationB","Trips A->B", "Trips B->A" );
        if(HTquery2 == NULL){
            exitCode=NOT_CREATED;
            handleErrors(exitCode,"No se pudo crear el archivo",TAD);
        }

    FILE * CSVquery3 = fopen("query3.csv", "w");
        if(CSVquery3 == NULL){
            exitCode=NOT_CREATED;
            handleErrors(exitCode,"No se pudo crear el archivo",TAD);
        }
    htmlTable HTquery3 = newTable("query3.html", 13, "J","F","M","A","M","J","J","A","S","O","N","D","Station");
        if(HTquery3 == NULL){
            exitCode=NOT_CREATED;
            handleErrors(exitCode,"No se pudo crear el archivo",TAD);
        }



// CARGO LA DATA DE STATIONS Y DE BIKES EN EL ADT  

    exitCode=readDataStations(dataStations,TAD);
    if(exitCode == NO_MEM){
        fclose(dataBikes);
        handleErrors(exitCode,"Error al leer y procesar los datos",TAD);
    }


    exitCode=readDataBikes(dataBikes,TAD);
    if(exitCode==NO_MEM){
        fclose(dataBikes);
        handleErrors(exitCode,"Error al leer y procesar los datos",TAD);
    }

// CARGO LOS QUERIES 

    exitCode=loadQuery1(TAD, CSVquery1, HTquery1);
    if(exitCode == NO_MEM || exitCode == PRINT_ERROR ){
        fclose(CSVquery1);
        closeHTMLTable(HTquery1);
        handleErrors(exitCode,"Error al realizar el query1",TAD);
    }


    exitCode=loadQuery2(TAD, CSVquery2, HTquery2);
    if(exitCode == NO_MEM || exitCode == PRINT_ERROR ){
        fclose(CSVquery2);
        closeHTMLTable(HTquery2);
        handleErrors(exitCode,"Error al realizar el query2",TAD);
    }    


    exitCode=loadQuery3(TAD, CSVquery3, HTquery3);
    if(exitCode == NO_MEM || exitCode == PRINT_ERROR ){
        fclose(CSVquery3);
        closeHTMLTable(HTquery3);
        handleErrors(exitCode,"Error al realizar el query3",TAD);
    }    
    handleErrors(OK,"Todo bien!!",TAD);            
}

    static char * intToString(size_t num) {
        int size = snprintf(NULL,0,"%ld",num);                  //Se calcula la cantidad de digitos de ADT
        char * str = malloc((size+1) * sizeof(char));
        if (str == NULL || errno == ENOMEM ) {
            return NULL;
        }
        snprintf(str,size+1,"%ld",num);
        return str;
    }


    int readDataStations(FILE * dataStations,bikeRentingADT TAD){
        int flag;
        size_t id;
        char delim[] = ";\n";
        int firstRead = 1;
        char buffer[BUFFER_LENGHT], * token, * name; 
        while( fgets(buffer,BUFFER_LENGHT,dataStations) != NULL){
            size_t j = 0;
            if( !firstRead ){
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
                flag=addStation(name,id,TAD);
                if(flag == -1 || errno == ENOMEM){
                    return NO_MEM;
                }
            }
        firstRead=0;
        }
    fclose(dataStations);
    }
    int readDataBikes( FILE * dataBikes,bikeRentingADT TAD){
        size_t idStart,idEnd;
        int flag;
        int month,isMember;
        int firstRead = 1;
        char buffer[BUFFER_LENGHT], * token;
        char delim[] = ";\n";
            while( fgets(buffer,BUFFER_LENGHT,dataBikes) != NULL){
                size_t j=0;
                if( !firstRead ){
                    token = strtok(buffer, delim);
                    while(token !=NULL){
                        if(j==DATE_COL){
                        sscanf(token,"%*d-%d-%*d",&month);
                        }else if(j==ID_START_COL){
                            idStart=atoi(token);
                        }else if(j==ID_END_COL){
                            idEnd=atoi(token);
                        }else if(j==ISMEMBER){
                            isMember=atoi(token);
                        }
                    token=strtok(NULL,delim);
                    j++;
                    }
                flag=processData(TAD,month,isMember,idStart,idEnd);
                if(flag == -1 || errno == ENOMEM){
                    return NO_MEM;
                }
                }
                firstRead=0;
            }
        return 0;
        fclose(dataBikes);
    }

    int loadQuery1 (bikeRentingADT ADT, FILE * CSVquery1, htmlTable HTquery1){
        int flag;
        size_t travels;
        char * name;
        char * travelString;
        toBeginQ1(ADT);
        flag=fprintf(CSVquery1, "Station;StartedTrips\n"); 
        if(flag < 0){
            return PRINT_ERROR;
        }
        while(hasNextQ1(ADT)){
            travels = getTripsQ1(ADT);
            name = getNameQ1(ADT);
            travelString = intToString(travels);
            if(name == NULL || travelString == NULL || errno == ENOMEM){
                free(travelString);
                free(name);                                                    
                return NO_MEM;
            }   
            addHTMLRow(HTquery1,name,travelString);                            
            flag = fprintf(CSVquery1,"%s;%zu\n",name, travels);                 
            if(flag < 0){
                return PRINT_ERROR;
            }
            free(travelString);
            free(name);
            nextQ1(ADT);
        }
        fclose(CSVquery1);
        closeHTMLTable(HTquery1);
        return OK;
    }

    int loadQuery2 (bikeRentingADT ADT, FILE * CSVquery2, htmlTable HTquery2){
        toBeginQ2(ADT);
        size_t travelsTo,travelsFrom;
        int flag;
        char * stationB,* stationA,* travelsFromString,* travelsToString;
        flag=fprintf(CSVquery2,"StationA;StationB;Trips A->B;Trips B->A\n");
        if(flag < 0){
            return PRINT_ERROR;
        }
        while(hasNextStartQ2(ADT)){
            stationA = getNameOfStartQ2(ADT);
            if(stationA == NULL){
                return NO_MEM;
            }
            while(hasNextDestinationQ2(ADT)){
                travelsTo = getTravelsToQ2(ADT);
                travelsFrom = getTravelsFromQ2(ADT);
                stationB = getNameOfDestinationQ2(ADT);
                if(stationB == NULL){
                    free(stationA);
                    return NO_MEM;
                }
                travelsToString = intToString(travelsTo);
                if(travelsToString == NULL){
                    return NO_MEM;    
                }
                travelsFromString = intToString(travelsFrom);
                if(travelsFromString == NULL){
                    return NO_MEM;
                }
                addHTMLRow(HTquery2,stationA,stationB,travelsToString,travelsFromString); 
                int flag = fprintf(CSVquery2,"%s;%s;%zu;%zu\n",stationA,stationB,travelsTo,travelsFrom); 
                free(travelsToString);
                free(travelsFromString);
                free(stationB);
                if( flag < 0){
                    return PRINT_ERROR;
                }
                nextDestinationQ2(ADT);
            }
            free(stationA);
            nextStartStationQ2(ADT);
        }
    fclose(CSVquery2);
    closeHTMLTable(HTquery2);
    return 0;
}

    int loadQuery3 (bikeRentingADT ADT, FILE * CSVquery3, htmlTable HTquery3){
        toBeginQ3(ADT);
        int flag;
        size_t travels [MONTHS];
        char * name;
        char * months[MONTHS];
        flag=fprintf(CSVquery3,"J;F;M;A;M;J;J;A;S;O;N;D;Station\n");
        if(flag < 0){
            return PRINT_ERROR;
        }
        while(hasNextQ3(ADT)){
            getTravelsByMonthQ3(ADT,travels);
            name = getNameQ3(ADT);
            if(name == NULL){
                return NO_MEM;
            }
            for( int i = 0; i < MONTHS ; i++){
                months[i] = intToString(travels[i]);
            }
            addHTMLRow(HTquery3,months[0],months[1],months[2],months[3],months[4],months[5],months[6],months[7],months[8],months[9],months[10],months[11],name); 
            flag = fprintf(CSVquery3,"%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%zu;%s\n",travels[0],travels[1],travels[2],travels[3],travels[4],travels[5],travels[6],travels[7],travels[8],travels[9],travels[10],travels[11],name); 
            if(flag < 0){
                return PRINT_ERROR;
            }
            for( int i = 0; i < MONTHS ; i++){
                free(months[i]);
            }
            free(name);
            nextQ3(ADT);
        }
        fclose(CSVquery3);
        closeHTMLTable(HTquery3);
        return 0;
    }

void handleErrors(bikeRentingADT ADT,mainErrors exitCode,char * Error){
    fprintf(stderr, Error);
    freeTad(ADT);
    exit(exitCode);
}

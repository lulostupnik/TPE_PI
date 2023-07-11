#include "bikeSharingADT.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htmlTable.h"
/*
Maximo de caracteres por linea.
*/
#define BUFFER_LENGHT 256
/*
Posicion de en el cual se encuentra el nombre de la estacion dentro del csv de stations.
*/
#define StationNameCol 0
/*
Posicion de en el cual se encuentra el id de la estacion dentro del csv de stations.
*/
#define StationIdcol 3
/*
Posicion de en el cual se encuentra la fecha de inicio del viaje dentro del csv de bikes.
*/
#define DATE_COL 0
/*
Posicion de en el cual se encuentra el id de la estacion de salida dentro del csv de bikes.
*/
#define ID_START_COL 1
/*
Posicion de en el cual se encuentra el id de la estacion de llegada dentro del csv de bikes.
*/
#define ID_END_COL 3
/*
Posicion de en el cual se encuentra la columna "Member" dentro del csv de bikes.
*/
#define ISMEMBER 5
/*
Enum de los distintos errores que podrian surgir a la hora de correr el programa.
*/
typedef enum {OK=0,WRONG_AMOUNT_OF_ARGUMENTS,WRONG_PATH,NOT_CREATED,NO_MEM,PRINT_ERROR} mainErrors;
/*
lee y separa la informacion del archivo de estaciones para enviarsela al TAD.
Devuelve 0 si lee y procesa los datos sin error.
Devuelve NO_MEM si hubo un error de lectura o procesado.
dataStations:archivo csv de estaciones.
TadStations:ADT.  
*/
int readDataStations(FILE * dataStations,bikeRentingADT TadStations);
/*
lee y separa la informacion del archivo de bicicletas para enviarsela al TAD.
Devuelve 0 si lee y procesa los datos sin error.
Devuelve NO_MEM si hubo un error de lectura o procesado.
dataBikes: archivo csv de bicicletas.
TadStations:ADT.  
*/
int readDataBikes( FILE * dataBikes,bikeRentingADT TadStations);
/*
Funcion para procesar todos los tipos de errores que se podrian generar y imprimir su explicacion en la salida
de errores.
*/
void handleErrors(mainErrors exitCode,const char * error,bikeRentingADT tad);
/*Las proximas 3 funciones cargan los datos a la tabla html y al csv con lo solicitado.
Devuelven 0  si no hay error,PRINT_ERROR en caso de haber un error a la hora de escribir en el csv o NO_MEM en caso de no poder copiar
el nombre de la estacion.
*/
int loadQuery3 (bikeRentingADT tad, FILE * CSVquery3, htmlTable HTquery3);
int loadQuery1 (bikeRentingADT tad, FILE * CSVquery2, htmlTable HTquery2);
int loadQuery2 (bikeRentingADT tad, FILE * CSVquery1, htmlTable HTquery1);

#define MONTHS 12 
#define CANT_QUERIES 3
#define CANT_CSV_TO_READ 2
/*
Funcion para cerrar todos los archivos csv.
*/
void closeCSV(FILE * fp[], int dim);
/*
Funcion para cerrar todos las tablas html.
*/
void closeHTML(htmlTable tableVec[], int dim);


int main(int argc, char * argv[]){
    errno = 0;
    int exitCode;
    // VALIDO LOS ARGUMENTOS QUE ME PASAN//
    if(argc != 3){
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

    //Creo un vector con los archivos, y a medida que los abro aumento la dim, para facilitar el cierre de los mismos en caso de error
    FILE * csvVec[CANT_QUERIES +  CANT_CSV_TO_READ];
    htmlTable htmlVec[CANT_QUERIES];
    int dimCsvVec = 0, dimHtmlVec = 0;

    FILE * dataStations = fopen(argv[2],"r"); 
    if(dataStations == NULL){
        exitCode=WRONG_PATH;
        handleErrors(exitCode,"Archivo no encontrado",TAD);
    }
    csvVec[dimCsvVec++] = dataStations;
    
    FILE * dataBikes = fopen(argv[1],"r"); 
    if(dataBikes == NULL){
        closeCSV(csvVec, dimCsvVec);
        exitCode=WRONG_PATH;
        handleErrors(exitCode,"Archivo no encontrado",TAD);
    }
    csvVec[dimCsvVec++] = dataBikes;

    FILE * CSVquery1 = fopen("query1.csv","w"); 
    if(CSVquery1 == NULL){
        closeCSV(csvVec, dimCsvVec);
        exitCode=NOT_CREATED;
        handleErrors(exitCode,"No se pudo crear el archivo",TAD);
    }
    csvVec[dimCsvVec++] = CSVquery1;
    htmlTable HTquery1 = newTable("query1.html", 2, "Station", "StartedTrips"); 
    if(HTquery1 == NULL){
        closeCSV(csvVec, dimCsvVec);
        exitCode=NOT_CREATED;
        handleErrors(exitCode,"No se pudo crear el archivo",TAD);
    }
    htmlVec[dimHtmlVec++] = HTquery1;

    FILE * CSVquery2 = fopen("query2.csv", "w"); 
    if(CSVquery2 == NULL){
        closeCSV(csvVec, dimCsvVec);
        closeHTML(htmlVec, dimHtmlVec);
        exitCode=NOT_CREATED;
        handleErrors(exitCode,"No se pudo crear el archivo",TAD);
    }
    csvVec[dimCsvVec++] = CSVquery2;

    htmlTable HTquery2 = newTable("query2.html", 4, "StationA","StationB","Trips A->B", "Trips B->A" );
    if(HTquery2 == NULL){
    
        exitCode=NOT_CREATED;
        handleErrors(exitCode,"No se pudo crear el archivo",TAD);
    }
    htmlVec[dimHtmlVec++] = HTquery2;

    FILE * CSVquery3 = fopen("query3.csv", "w");
    if(CSVquery3 == NULL){
        closeCSV(csvVec, dimCsvVec);
        closeHTML(htmlVec, dimHtmlVec);
        exitCode=NOT_CREATED;
        handleErrors(exitCode,"No se pudo crear el archivo",TAD);
    }
    csvVec[dimCsvVec++] = CSVquery3;
    htmlTable HTquery3 = newTable("query3.html", 13, "J","F","M","A","M","J","J","A","S","O","N","D","Station");
    if(HTquery3 == NULL){
        closeCSV(csvVec, dimCsvVec);
        closeHTML(htmlVec, dimHtmlVec);
        exitCode=NOT_CREATED;
        handleErrors(exitCode,"No se pudo crear el archivo",TAD);
    }
    htmlVec[dimHtmlVec++] = HTquery3;
    
    // CARGO LA DATA DE STATIONS Y DE BIKES EN EL ADT  

    exitCode=readDataStations(dataStations,TAD);
    if(exitCode == NO_MEM){
        closeCSV(csvVec, dimCsvVec);
        closeHTML(htmlVec, dimHtmlVec);
        handleErrors(exitCode,"Error al leer y procesar los datos",TAD);
    }
    exitCode=readDataBikes(dataBikes,TAD);
    if(exitCode==NO_MEM){
        closeCSV(csvVec, dimCsvVec);
        closeHTML(htmlVec, dimHtmlVec);
        handleErrors(exitCode,"Error al leer y procesar los datos",TAD);
    }
    // CARGO LOS QUERIES 
    exitCode=loadQuery1(TAD, CSVquery1, HTquery1);
    if(exitCode == NO_MEM || exitCode == PRINT_ERROR ){
        closeCSV(csvVec, dimCsvVec);
        closeHTML(htmlVec, dimHtmlVec);
        handleErrors(exitCode,"Error al realizar el query1",TAD);
    }
    exitCode=loadQuery2(TAD, CSVquery2, HTquery2);
    if(exitCode == NO_MEM || exitCode == PRINT_ERROR ){
        closeCSV(csvVec, dimCsvVec);
        closeHTML(htmlVec, dimHtmlVec);
        handleErrors(exitCode,"Error al realizar el query2",TAD);
    }    
    exitCode=loadQuery3(TAD, CSVquery3, HTquery3);
    closeCSV(csvVec, dimCsvVec);
    closeHTML(htmlVec, dimHtmlVec);
    freeTad(TAD);
    if(exitCode == NO_MEM || exitCode == PRINT_ERROR ){
        handleErrors(exitCode,"Error al realizar el query3",TAD);
    } 
 
    return 0;           
}

static char * sizetToString(size_t num) {
    int size = snprintf(NULL,0,"%ld",num);                  //Se calcula la cantidad de digitos del num
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
            flag=addStation(TAD,name,id);
            if(flag == -1 || errno == ENOMEM){
                return NO_MEM;
            }
        }
    firstRead=0;
    }
return 0;
}
int readDataBikes( FILE * dataBikes,bikeRentingADT ADT){
    size_t idStart,idEnd;
    int month,isMember;
    int flag;
    size_t firstRead = 1;
    char buffer[BUFFER_LENGHT],*token;
    char delim[]=";\n";
    while(fgets(buffer,BUFFER_LENGHT,dataBikes)!=NULL){
        size_t j=0;
        if(!firstRead){
            token = strtok(buffer, delim);
            while(token !=NULL){
                if(j==DATE_COL){
                    sscanf(token,"%*d-%d-%*d",&month);
                }else if(j==ID_START_COL){
                    idStart=atoi(token);
                }else if(j==ID_END_COL){
                    idEnd=atoi(token);
                }else if(j==ISMEMBER){
                    if(strcmp(token,"member") == 0){
                        isMember = 1;
                    }else{
                        isMember = 0;
                    }
                }
                token = strtok(NULL,delim);
                j++;
            }
           flag=processData(ADT,month,isMember,idStart,idEnd);
             if(flag == -1 || errno == ENOMEM){
                return NO_MEM;
            }
        }
        firstRead=0;  
    }
    return 0;
}

int loadQuery1 (bikeRentingADT ADT, FILE * CSVquery1, htmlTable HTquery1){
    int flag;
    size_t travels;
    char * name;
    char * travelString;
    toBeginQ1(ADT);
    flag=fprintf(CSVquery1,"Station;StartedTrips\n"); 
    if(flag < 0){
        return PRINT_ERROR;
    }
    while(hasNextQ1(ADT)){
        travels = getTripsQ1(ADT);
        name = getNameQ1(ADT);
        travelString = sizetToString(travels);
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
            travelsTo = getTripsToQ2(ADT);
            travelsFrom = getTripsFromQ2(ADT);
            stationB = getNameOfDestinationQ2(ADT);
            if(stationB == NULL){
                free(stationA);
                return NO_MEM;
            }
            travelsToString = sizetToString(travelsTo);
            if(travelsToString == NULL){
                return NO_MEM;    
            }
            travelsFromString = sizetToString(travelsFrom);
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
        getTripsByMonthQ3(ADT,travels);
        name = getNameQ3(ADT);
        if(name == NULL){
            return NO_MEM;
        }
        for( int i = 0; i < MONTHS ; i++){
            months[i] = sizetToString(travels[i]);
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
    return 0;
}
void handleErrors(mainErrors exitCode, const char * Error,bikeRentingADT ADT){
    fprintf(stderr,"%s\n",Error);
    freeTad(ADT);
    exit(exitCode);
}

void closeCSV(FILE * fp[], int dim){
    for(int i=0; i<dim; i++){
        fclose(fp[i]);
    }
}
void closeHTML(htmlTable tableVec[], int dim){
    for(int i=0; i<dim; i++){
        closeHTMLTable(tableVec[i]);
    }
}


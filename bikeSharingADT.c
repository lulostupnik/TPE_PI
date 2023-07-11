#include "ADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <string.h>


#define MONTHS 12

typedef struct cols{
    char * name;
    size_t nameLen;
    size_t tripsTo;
    size_t tripsFrom;
}tCols;

typedef struct rows{
   tCols * cols;
   char * name;
   size_t nameLen;
}tRows;


typedef struct station{
    char * name;
    size_t nameLen;
    size_t stationId;
    size_t tripsByMembers;
    size_t monthsVec[MONTHS];
}tStation;


typedef enum {UNORDERED=0, ID, NAME, TRIPS} tOrder;

typedef struct iterators{
    size_t q1_i;
    size_t q2_i;
    size_t q2_j;
    size_t q3_i;
}tIterators;


typedef struct bikeRentingCDT{
    tStation * vecStations;
    size_t stationQty;


    tRows * matrix;
    size_t oldSizeOfmatrix;
    
    size_t firstRead;
    tOrder order;
    tIterators iterators;

}tBikeRentingCDT;


static int compare_trips(const void * a, const void * b);
static int compare_cols(const void *a, const void *b);
static int compare_stations(const void * a, const void * b);
static int compare_matrix(const void *a, const void *b) ;
static int compare_ids(const void * a, const void * b);

static void sortByName(bikeRentingADT ADT);
static void sortByTrips(bikeRentingADT ADT);

static int searchRepeatedId(tStation * vec,size_t size, size_t id);
static void  fillMatrixWithzeros(tRows * matrix, size_t dim,size_t * oldSize);
static int matrixColsAlloc(tRows * matrix, size_t dim,size_t * oldSize);
static int  binarySearch(tStation * vec, int min, int max, size_t Id);
static void startMonthsInZero(size_t months[MONTHS]);




static int compare_trips(const void * a, const void * b){
    int c;
    const tStation * station1 = (const tStation *) a;
    const tStation * station2 = (const tStation *) b;
    c = station2->tripsByMembers - station1->tripsByMembers;
    if(c == 0){
    return strcasecmp(station1->name,station2->name);
    }
    return c;
}
static void sortByTrips(bikeRentingADT ADT){
    qsort(ADT->vecStations,ADT->stationQty,sizeof(tStation),compare_trips); 
    ADT->order = TRIPS;
}


static int searchRepeatedId(tStation * vec,size_t size, size_t id){
    for( size_t i = 0; i < size; i++){
        if( vec[i].stationId == id){
            return 1;
        }
    }
    return -1;
}


static void  fillMatrixWithzeros(tRows * matrix, size_t dim,size_t * oldSize){
    tCols aux = {NULL,0,0,0};
    for (size_t i = 0; i < *oldSize;i++){
        for (size_t j = *oldSize; j < dim;j++){
            matrix[i].cols[j] = matrix[j].cols[i] = aux;
        }
    }
    
    for(size_t i=*oldSize;i<dim;i++){
        for(size_t j=*oldSize;j<dim;j++){
            matrix[i].cols[j] = aux;
        }
    }
}

static int matrixColsAlloc(tRows * matrix, size_t dim,size_t * oldSize){
    errno = 0;

    tCols * auxDestino;
        for( size_t i=0; i < dim; i++){
            auxDestino=realloc(matrix[i].cols,dim * sizeof(tCols));
            if(auxDestino == NULL || errno == ENOMEM){                    // si me quedo sin memoria libero lo ya reallocado.
                for(int j=0; j < i;j++){
                    free(matrix[j].cols);
                }
                free(auxDestino); //errno test
                return 0;
            }
            matrix[i].cols=auxDestino;
        }
    fillMatrixWithzeros(matrix,dim,oldSize);
    *oldSize=dim;
    return 1;
}


static int  binarySearch(tStation * vec, int min, int max, size_t Id) {
     if (min > max){
        return -1;
    }
    while (min <= max) {
        int mid = min+(max - min)/2;
        if (vec[mid].stationId == Id)
            return mid;
        if (vec[mid].stationId < Id)
            min = mid + 1;
        else
            max = mid - 1;
    }
    return -1;
}

static void startMonthsInZero(size_t months[MONTHS]){
    for( size_t i = 0; i < MONTHS; i++){
        months[i] = 0;
    }
}

static int compare_ids(const void * a, const void * b){
    
    const tStation * station1 = (const tStation *) a;
    const tStation * station2 = (const tStation *) b;
    return (station1->stationId-station2->stationId);
}

static void sortByName(bikeRentingADT ADT){
    qsort(ADT->vecStations,ADT->stationQty,sizeof(tStation),compare_stations);
    qsort(ADT->matrix,ADT->oldSizeOfmatrix,sizeof(tRows),compare_matrix);
       for( size_t i = 0; i < ADT->oldSizeOfmatrix; i++){
        qsort(ADT->matrix[i].cols,ADT->oldSizeOfmatrix,sizeof(tCols),compare_cols);
        }
    ADT->order = NAME;
}


static int compare_stations(const void * a, const void * b) {
    const tStation * station1 = (const tStation *) a;
    const tStation * station2 = (const tStation *) b;
    return strcasecmp(station1->name, station2->name);
}


static int compare_cols(const void *a, const void *b){
     const tCols*station1 = (const tCols *)a;
        const tCols* station2 = (const tCols *)b;
       if(station1->name== NULL){
        return 1;
       }
        if(station2->name== NULL){
        return -1;
        }
         return strcasecmp(station1->name, station2->name);
}

static int compare_matrix(const void *a, const void *b){
    const tRows *station1 = (const tRows *)a;
    const tRows *station2 = (const tRows *)b;
    if(station1->name== NULL){
        return 1;
    }
    if(station2->name== NULL){
        return -1;
    }
    return strcasecmp(station1->name,station2->name);
}

void orderByids(bikeRentingADT ADT){
    qsort(ADT->vecStations,ADT->stationQty,sizeof(tStation),compare_ids);
    ADT->order = ID;
}

bikeRentingADT newBikesRenting(void){
    errno = 0;
   return calloc(1, sizeof(tBikeRentingCDT));
}

size_t getNumberOfStations(bikeRentingADT ADT){
    return ADT->stationQty;
}


int addStation(bikeRentingADT ADT, char *name,size_t id) {
    errno = 0;
    if(searchRepeatedId(ADT->vecStations,ADT->stationQty,id) == -1){   // si no encuentra la estacion la agrega       
        size_t nameLenAux = strlen(name);
        char * nameAux = malloc(nameLenAux + 1);
        if(nameAux == NULL || errno == ENOMEM){
            free(nameAux); //Lo pongo para testear cambiando el valor de errno (El caso que errno sea ENOMEM, pero nameAux no es NULL)
            // errno test
            return -1;
        }
        strcpy(nameAux, name);
        
        tStation * aux =  realloc(ADT->vecStations,(ADT->stationQty+1) * sizeof(tStation));
        if(aux == NULL || errno == ENOMEM){
            free(aux); //testeo
            free(nameAux);
            return -1;
        }

        //Cuando ya se hizo el realloc del vector, igualamos el puntero dentro del mismo a los aux
        aux[ADT->stationQty].nameLen = nameLenAux;
        aux[ADT->stationQty].name = nameAux;
        aux[ADT->stationQty].stationId = id;
        startMonthsInZero(aux[ADT->stationQty].monthsVec);
        aux[ADT->stationQty].tripsByMembers = 0;
        
        ADT->vecStations = aux;
        ADT->stationQty++;
        
        //Seteo el flag del adt
        ADT->firstRead=1;
        ADT->order = UNORDERED;
        return 0;
    }
    return 1;
}

int processData(bikeRentingADT ADT,int month,int isMember,size_t idStart,size_t idEnd){
    errno = 0;
    if(ADT->order != ID){
         orderByids(ADT);
    }
    int newIdStart = binarySearch(ADT->vecStations,0,ADT->stationQty-1,idStart);
    int newIdEnd  = binarySearch(ADT->vecStations,0,ADT->stationQty-1,idEnd);
    //Si alguno de las dos estaciones no existe, no agregamos el viaje.
    if( newIdEnd == -1 || newIdStart == -1 || month < 1 || month > 12) {
        return 1;
    }

    if( ADT->firstRead == 1){

        tRows * aux;
       
        aux = realloc(ADT->matrix,ADT->stationQty * sizeof(tRows));
        if(aux == NULL || errno == ENOMEM){                    // si me quedo sin memoria libero lo ya reallocado.
            return -1;
        }

        tRows rowsAux = {NULL, NULL, 0};
        //Lleno con ceros las filas que agregue en el realloc
        for(int i=(ADT->oldSizeOfmatrix);i<ADT->stationQty;i++){
            aux[i] = rowsAux;
        }

        if(!matrixColsAlloc(aux,ADT->stationQty,&(ADT->oldSizeOfmatrix))){
            free(aux);
            return -1;
        }
        ADT->matrix=aux;
        ADT->firstRead = 0;
    }
 
    if(isMember){
        ADT->vecStations[newIdStart].tripsByMembers++;
    }
    
    ADT->vecStations[newIdStart].monthsVec[month-1]++;;

    if( newIdEnd != newIdStart){ //Solo agrego el viaje a la matrix si no es un viaje circular
        char * nameEnd = ADT->vecStations[newIdEnd].name;
        char * nameStart = ADT->vecStations[newIdStart].name;
        if( ADT->matrix[newIdStart].cols[newIdEnd].tripsTo==0 && ADT->matrix[newIdStart].cols[newIdEnd].tripsFrom == 0){
            
            //Lo guardo dentro de la matrix, para ver las llegadas
            ADT->matrix[newIdStart].cols[newIdEnd].name=nameEnd;
            ADT->matrix[newIdStart].cols[newIdEnd].nameLen=ADT->vecStations[newIdEnd].nameLen;

            ADT->matrix[newIdEnd].cols[newIdStart].nameLen=ADT->vecStations[newIdStart].nameLen;
            ADT->matrix[newIdEnd].cols[newIdStart].name = nameStart;
            
            //Lo guardamos en las "filas", para cuando itero en la estacion A (A--> todas las que llega)
            ADT->matrix[newIdStart].name=nameStart;
            ADT->matrix[newIdEnd].name = nameEnd;
            ADT->matrix[newIdStart].nameLen =ADT->vecStations[newIdStart].nameLen;
            ADT->matrix[newIdEnd].nameLen =ADT->vecStations[newIdEnd].nameLen;
        }
        ADT->matrix[newIdStart].cols[newIdEnd].tripsTo++;
        ADT->matrix[newIdEnd].cols[newIdStart].tripsFrom++;
    }
    return 0;
}



//Iteradores para que el front pueda hacer los queries:

void toBeginQ1(bikeRentingADT ADT){
    if(ADT->order != TRIPS){
        sortByTrips(ADT);
    }
    
    ADT->iterators.q1_i = 0;
}

int hasNextQ1(bikeRentingADT ADT){
    return ADT->iterators.q1_i < ADT->stationQty;
}


//Devuelve NULL si no hay memoria para copiarlo, o si no hay next.
char * getNameQ1(bikeRentingADT ADT){
    errno = 0;
    if(!hasNextQ1(ADT)){
        return NULL;
    }
    char * aux = malloc(ADT->vecStations[ADT->iterators.q1_i].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
        free(aux); //errno test
        return NULL;
    }
    return strcpy(aux, ADT->vecStations[ADT->iterators.q1_i].name);
}

size_t getTripsQ1(bikeRentingADT ADT){
    if(!hasNextQ1(ADT)){
        return -1; // Le retornamos basura (aunque sea size_t, que devuelva lo que sea -1)
    }
    return ADT->vecStations[ADT->iterators.q1_i].tripsByMembers;
}


void nextQ1(bikeRentingADT ADT){
    ADT->iterators.q1_i++;
}




//Q2:


void toBeginQ2(bikeRentingADT ADT){
    
    if(ADT->order != NAME){
        sortByName(ADT);
    }
    ADT->iterators.q2_i = 0;
    ADT->iterators.q2_j = 0;

}

int hasNextStartQ2(bikeRentingADT ADT){
    if(ADT->iterators.q2_i < ADT->oldSizeOfmatrix && ADT->matrix[ADT->iterators.q2_i].name != NULL){
        return 1;
    }
    return 0;//ADT->iterators.q2_i < ADT->oldSizeOfmatrix;
}

int hasNextDestinationQ2(bikeRentingADT ADT){
    /*
    ** Solo comparamos con NULL, ya que la lista de destinos esta ordenada para que si no hay una llegada, se ponga el "Nombre" al final, el cual es NULL. 
    ** Es decir, una vez que encuentra NULL, ya no hay mas viajes. 
    ** Tampoco hay que comparar para que no devuelva las estaciones que no tienen viajes entre si, ya que en ese caso, nunca se cambiaria el puntero del nombre, y seguiria siendo NULL.
    ** Idem con los viajes circulares 
    ** (Ver processData y compareMatrix).
    */
    if(ADT->iterators.q2_j >= ADT->oldSizeOfmatrix || ADT->matrix[ADT->iterators.q2_i].cols[ADT->iterators.q2_j].name == NULL){
                return 0;
    } 
    return 1;
}



size_t getTripsToQ2(bikeRentingADT ADT){
    return ADT->matrix[ADT->iterators.q2_i].cols[ADT->iterators.q2_j].tripsTo;
}
size_t getTripsFromQ2(bikeRentingADT ADT){
    return ADT->matrix[ADT->iterators.q2_i].cols[ADT->iterators.q2_j].tripsFrom;
}

char * getNameOfDestinationQ2(bikeRentingADT ADT){
    if(!hasNextStartQ2(ADT) || !hasNextDestinationQ2(ADT)){
        return NULL;
    }
    char * aux = malloc(ADT->matrix[ADT->iterators.q2_i].cols[ADT->iterators.q2_j].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
        free(aux); // errno test
       return NULL;
    }
    return strcpy(aux, ADT->matrix[ADT->iterators.q2_i].cols[ADT->iterators.q2_j].name);

}

char * getNameOfStartQ2(bikeRentingADT ADT){
    errno = 0;
     if(!hasNextStartQ2(ADT)){
        return NULL;
    }
    char * aux = malloc(ADT->matrix[ADT->iterators.q2_i].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
        free(aux); //errno test
        return NULL;
    }
    return strcpy(aux, ADT->matrix[ADT->iterators.q2_i].name);
}

void nextStartStationQ2(bikeRentingADT ADT){
    ADT->iterators.q2_i++;
    ADT->iterators.q2_j=0;
}

void nextDestinationQ2(bikeRentingADT ADT){
    ADT->iterators.q2_j++;
}




//Q3

void toBeginQ3(bikeRentingADT ADT){
    if(ADT->order != NAME){
          sortByName(ADT);
    }
    ADT->iterators.q3_i = 0;
}
int hasNextQ3(bikeRentingADT ADT){
    return ADT->iterators.q3_i < ADT->stationQty;
}

char * getNameQ3(bikeRentingADT ADT){
    errno = 0;
    if(!hasNextQ3(ADT)){
        return NULL;
    }
    char * aux = malloc(ADT->vecStations[ADT->iterators.q3_i].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
        free(aux); //errno test
        return NULL;
    }
    return strcpy(aux, ADT->vecStations[ADT->iterators.q3_i].name);
}

void nextQ3(bikeRentingADT ADT){
    ADT->iterators.q3_i++;
}


void getTripsByMonthQ3(bikeRentingADT ADT,size_t tripsVec[]){
    for( size_t i = 0; i < MONTHS; i++){
        tripsVec[i] = ADT->vecStations[ADT->iterators.q3_i].monthsVec[i];
    }
}

void freeTad(bikeRentingADT ADT) {
    if(ADT != NULL){
        for( size_t i = 0; i < ADT->stationQty; i++) {
            free(ADT->vecStations[i].name);   
        }
        for(size_t i = 0 ;i <ADT->oldSizeOfmatrix;i++){
            free(ADT->matrix[i].cols);
        }
        free(ADT->matrix);
        free(ADT->vecStations);
        free(ADT);
}
}
#include "ADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#define BLOCK 500
#define MONTHS 12

typedef struct destino{
    char * name;
    size_t nameLen;
    size_t travelsTo;
    size_t travelsFrom;
}tDestino;

typedef struct matrix{
   tDestino* Travels;
   char * name;
   size_t nameLen;
}tMatrix;



typedef struct station{
    char * stationName;
    size_t nameLen;
    size_t stationId;
    size_t travelsByMembers;
    size_t Months[MONTHS];
}tStation;

typedef struct station * vec;

typedef enum {UNORDERED=0, ID, NAME, TRIPS} tOrder;

typedef struct iterators{
    size_t q1_i;
    size_t q2_i;
    size_t q2_j;
    size_t q3_i;
}tIterators;


typedef struct bikeRentingCDT{
    vec Stations;
    size_t stationQty;
    tMatrix* matriz;
    size_t firstRead;
    tOrder order;
    tIterators iterators;
    size_t oldSizeOfMatriz;
}tBikeRentingCDT;


static int compare_Travels(const void * a, const void * b);
static void sortByTrips(bikeRentingADT ADT);
static int compare_ids(const void * a, const void * b);
static int SearchForRepeated(vec vec,size_t size, size_t id);
static void  fillWithzeros(tMatrix * matriz, size_t dim,size_t * oldSize);
static int matrizMalloc(tMatrix * matriz, size_t dim,size_t * oldSize);
static int  binarySearch(vec vec, int min, int max, size_t Id);
static void startMonthsInZero(size_t months[MONTHS]);
static int compare_destinos(const void *a, const void *b);
static int compare_stations(const void * a, const void * b);
static int compare_matrix(const void *a, const void *b) ;
static void sortByName(bikeRentingADT ADT);



static int compare_Travels(const void * a, const void * b){
    int c;
    const vec station1 = (const vec) a;
    const vec station2 = (const vec) b;
    c = station2->travelsByMembers - station1->travelsByMembers;
    if(c == 0){
    return strcasecmp(station1->stationName,station2->stationName);
    }
    return c;
}
static void sortByTrips(bikeRentingADT ADT){
    qsort(ADT->Stations,ADT->stationQty,sizeof(tStation),compare_Travels); 
    ADT->order = TRIPS;
}
size_t getNumberOfStations(bikeRentingADT ADT){
    return ADT->stationQty;
}


static int SearchForRepeated(vec vec,size_t size, size_t id){
    for( size_t i = 0; i < size; i++){
        if( vec[i].stationId == id){
            return 1;
        }
    }
    return -1;
}


static void  fillWithzeros(tMatrix * matriz, size_t dim,size_t * oldSize){
    tDestino aux = {NULL,0,0,0};
    for (size_t i = 0; i < *oldSize;i++){
        for (size_t j = *oldSize; j < dim;j++){
            matriz[i].Travels[j] = matriz[j].Travels[i] = aux;
        }
    }
    
    for(size_t i=*oldSize;i<dim;i++){
        for(size_t j=*oldSize;j<dim;j++){
            matriz[i].Travels[j] = aux;
        }
    }
}


static int matrizMalloc(tMatrix * matriz, size_t dim,size_t * oldSize){
    errno = 0;
    tMatrix aux = {NULL, NULL, 0};
    for(int i=*oldSize;i<dim;i++){
        matriz[i] = aux;
    }

    tDestino * auxDestino;
        for( size_t i=0; i < dim; i++){
            auxDestino=realloc(matriz[i].Travels,dim * sizeof(tDestino));
            if(auxDestino == NULL || errno == ENOMEM){                    // si me quedo sin memoria libero lo ya reallocado.
                for(int j=0; j < i;j++){
                    free(matriz[j].Travels); // ARREGLAR
                }
                free(auxDestino); //errno test
                return 0;
            }
            matriz[i].Travels=auxDestino;
        }
    fillWithzeros(matriz,dim,oldSize);
    *oldSize=dim;
    return 1;
}


static int  binarySearch(vec vec, int min, int max, size_t Id) {
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
    qsort(ADT->Stations,ADT->stationQty,sizeof(tStation),compare_stations);
    qsort(ADT->matriz,ADT->oldSizeOfMatriz,sizeof(tMatrix),compare_matrix);
       for( size_t i = 0; i < ADT->oldSizeOfMatriz; i++){
        qsort(ADT->matriz[i].Travels,ADT->oldSizeOfMatriz,sizeof(tDestino),compare_destinos);
        }
    ADT->order = NAME;
}


static int compare_stations(const void * a, const void * b) {
    const vec station1 = (const vec) a;
    const vec station2 = (const vec) b;
    return strcasecmp(station1->stationName, station2->stationName);
}


static int compare_destinos(const void *a, const void *b){
     const tDestino*station1 = (const tDestino *)a;
        const tDestino* station2 = (const tDestino *)b;
       if(station1->name== NULL){
        return 1;
       }
        if(station2->name== NULL){
        return -1;
        }
         return strcasecmp(station1->name, station2->name);
}

static int compare_matrix(const void *a, const void *b){
    const tMatrix *station1 = (const tMatrix *)a;
    const tMatrix *station2 = (const tMatrix *)b;
    if(station1->name== NULL){
        return 1;
    }
    if(station2->name== NULL){
        return -1;
    }
    return strcasecmp(station1->name,station2->name);
}

void orderByids(bikeRentingADT ADT){
    qsort(ADT->Stations,ADT->stationQty,sizeof(tStation),compare_ids);
    ADT->order = ID;
}

bikeRentingADT newBikesRenting(void){
    errno = 0;
   return calloc(1, sizeof(tBikeRentingCDT));
}

int addStation(char *name,size_t id,bikeRentingADT ADT) {
    errno = 0;
    if(SearchForRepeated(ADT->Stations,ADT->stationQty,id) == -1){   // si no encuentra la estacion la agrega       
        size_t nameLenAux = strlen(name);
        char * nameAux = malloc(nameLenAux + 1);
        if(nameAux == NULL || errno == ENOMEM){
            free(nameAux); //Lo pongo para testear cambiando el valor de errno (El caso que errno sea ENOMEM, pero nameAux no es NULL)
            // errno test
            return -1;
        }
        strcpy(nameAux, name);
        
        vec aux =  realloc(ADT->Stations,(ADT->stationQty+1) * sizeof(tStation));
        if(aux == NULL || errno == ENOMEM){
            free(aux); //testeo
            free(nameAux);
            return -1;
        }

        //Cuando ya se hizo el realloc del vector, igualamos el puntero dentro del mismo a los aux
        aux[ADT->stationQty].nameLen = nameLenAux;
        aux[ADT->stationQty].stationName = nameAux;
        aux[ADT->stationQty].stationId = id;
        startMonthsInZero(aux[ADT->stationQty].Months);
        aux[ADT->stationQty].travelsByMembers = 0;
        
        ADT->Stations = aux;
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
    int flag;
    
    if(ADT->order != ID){
         orderByids(ADT);
    }
    int newIdStart = binarySearch(ADT->Stations,0,ADT->stationQty-1,idStart);
    int newIdEnd  = binarySearch(ADT->Stations,0,ADT->stationQty-1,idEnd);
    //Si alguno de las dos estaciones no existe, no agregamos el viaje.
    if( newIdEnd == -1 || newIdStart == -1 || month < 1 || month > 12) {
        return 1;
    }

    if( ADT->firstRead == 1){

        tMatrix * aux;
       
        aux = realloc( ADT->matriz,ADT->stationQty * sizeof(tMatrix));
        if(aux == NULL || errno == ENOMEM){                    // si me quedo sin memoria libero lo ya reallocado.
            free(aux); // errno test
            return -1;
        }
        
        if(!matrizMalloc(aux,ADT->stationQty,&(ADT->oldSizeOfMatriz))){
            free(aux);
            return -1;
        }
        ADT->matriz=aux;
        ADT->firstRead = 0;
    }
 
    if(isMember){
        ADT->Stations[newIdStart].travelsByMembers++;
    }
    
    ADT->Stations[newIdStart].Months[month-1]++;;

    if( newIdEnd != newIdStart){ //Solo agrego el viaje a la matriz si no es un viaje circular
        char * nameEnd = ADT->Stations[newIdEnd].stationName;
        char * nameStart = ADT->Stations[newIdStart].stationName;
        if( ADT->matriz[newIdStart].Travels[newIdEnd].travelsTo==0 && ADT->matriz[newIdStart].Travels[newIdEnd].travelsFrom == 0){
            
            //Lo guardo dentro de la matriz, para ver las llegadas
            ADT->matriz[newIdStart].Travels[newIdEnd].name=nameEnd;
            ADT->matriz[newIdStart].Travels[newIdEnd].nameLen=ADT->Stations[newIdEnd].nameLen;

            ADT->matriz[newIdEnd].Travels[newIdStart].nameLen=ADT->Stations[newIdStart].nameLen;
            ADT->matriz[newIdEnd].Travels[newIdStart].name = nameStart;
            
            //Lo guardamos en las "filas", para cuando itero en la estacion A (A--> todas las que llega)
            ADT->matriz[newIdStart].name=nameStart;
            ADT->matriz[newIdEnd].name = nameEnd;
            ADT->matriz[newIdStart].nameLen =ADT->Stations[newIdStart].nameLen;
            ADT->matriz[newIdEnd].nameLen =ADT->Stations[newIdEnd].nameLen;
        }
        ADT->matriz[newIdStart].Travels[newIdEnd].travelsTo++;
        ADT->matriz[newIdEnd].Travels[newIdStart].travelsFrom++;
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
    char * aux = malloc(ADT->Stations[ADT->iterators.q1_i].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
        free(aux); //errno test
        return NULL;
    }
    return strcpy(aux, ADT->Stations[ADT->iterators.q1_i].stationName);
}

size_t getTripsQ1(bikeRentingADT ADT){
    if(!hasNextQ1(ADT)){
        return -1; // Le retornamos basura (aunque sea size_t, que devuelva lo que sea -1)
    }
    return ADT->Stations[ADT->iterators.q1_i].travelsByMembers;
}


void nextQ1(bikeRentingADT ADT){
    ADT->iterators.q1_i++;
}







void toBeginQ2(bikeRentingADT ADT){
    
    if(ADT->order != NAME){
        sortByName(ADT);
    }
    ADT->iterators.q2_i = 0;
    ADT->iterators.q2_j = 0;

}

int hasNextStartQ2(bikeRentingADT ADT){
    if(ADT->iterators.q2_i < ADT->oldSizeOfMatriz && ADT->matriz[ADT->iterators.q2_i].name != NULL){
        return 1;
    }
    return 0;//ADT->iterators.q2_i < ADT->oldSizeOfMatriz;
}

int hasNextDestinationQ2(bikeRentingADT ADT){
    /*
    ** Solo comparamos con NULL, ya que la lista de destinos esta ordenada para que si no hay una llegada, se ponga el "Nombre" al final, el cual es NULL. 
    ** Es decir, una vez que encuentra NULL, ya no hay mas viajes. 
    ** Tampoco hay que comparar para que no devuelva las estaciones que no tienen viajes entre si, ya que en ese caso, nunca se cambiaria el puntero del nombre, y seguiria siendo NULL.
    ** Idem con los viajes circulares 
    ** (Ver processData y compareMatrix).
    */
    if(ADT->iterators.q2_j >= ADT->oldSizeOfMatriz || ADT->matriz[ADT->iterators.q2_i].Travels[ADT->iterators.q2_j].name == NULL){
                return 0;
    } 
    return 1;
}



size_t getTravelsToQ2(bikeRentingADT ADT){
    return ADT->matriz[ADT->iterators.q2_i].Travels[ADT->iterators.q2_j].travelsTo;
}
size_t getTravelsFromQ2(bikeRentingADT ADT){
    return ADT->matriz[ADT->iterators.q2_i].Travels[ADT->iterators.q2_j].travelsFrom;
}

char * getNameOfDestinationQ2(bikeRentingADT ADT){
    if(!hasNextStartQ2(ADT) || !hasNextDestinationQ2(ADT)){
        return NULL;
    }
    char * aux = malloc(ADT->matriz[ADT->iterators.q2_i].Travels[ADT->iterators.q2_j].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
        free(aux); // errno test
       return NULL;
    }
    return strcpy(aux, ADT->matriz[ADT->iterators.q2_i].Travels[ADT->iterators.q2_j].name);

}

char * getNameOfStartQ2(bikeRentingADT ADT){
    errno = 0;
     if(!hasNextStartQ2(ADT)){
        return NULL;
    }
    char * aux = malloc(ADT->matriz[ADT->iterators.q2_i].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
        free(aux); //errno test
        return NULL;
    }
    return strcpy(aux, ADT->matriz[ADT->iterators.q2_i].name);
}

void nextStartStationQ2(bikeRentingADT ADT){
    ADT->iterators.q2_i++;
    ADT->iterators.q2_j=0;
}

void nextDestinationQ2(bikeRentingADT ADT){
    ADT->iterators.q2_j++;
}













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
    char * aux = malloc(ADT->Stations[ADT->iterators.q3_i].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
        free(aux); //errno test
        return NULL;
    }
    return strcpy(aux, ADT->Stations[ADT->iterators.q3_i].stationName);
}

void nextQ3(bikeRentingADT ADT){
    ADT->iterators.q3_i++;
}


void getTravelsByMonthQ3(bikeRentingADT ADT,size_t travels[]){
    for( size_t i = 0; i < MONTHS; i++){
        travels[i] = ADT->Stations[ADT->iterators.q3_i].Months[i];
    }
}



void freeTad(bikeRentingADT ADT) {
    for( size_t i = 0; i < ADT->stationQty; i++) {
        free(ADT->Stations[i].stationName);   
    }
    for(size_t i = 0 ;i <ADT->oldSizeOfMatriz;i++){
        free(ADT->matriz[i].Travels);
    }
    free(ADT->matriz);
    free(ADT->Stations);
    free(ADT);
}
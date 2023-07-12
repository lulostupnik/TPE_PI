#include "bikeSharingADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <string.h>

#define MONTHS 12

/*
    Estructura de las columnas de la matriz dinamica
*/
typedef struct cols{
    char * name;                    //Nombra de la estacion de llegada (La Columna)
    size_t nameLen;                 // Longitud del nombre de la estacion de llegada
    size_t tripsTo;                 // Viajes hacia la estacion de llegada (desde la de salida)
    size_t tripsFrom;               // Viajes desde la estacion de llegada (hacia la de salida)             
}tCols;

/*
    Estructura de las filas de la matriz dinamica 
*/
typedef struct rows{
   tCols * cols;                     // "Vector" de estaciones de llegada. 
   char * name;                     // Nombre de la Estacion de Salida (La Fila)
   size_t nameLen;                  // Longitud del nombre de la estacion de Salida                  
}tRows;

/*
    Estructura de cada estacion. 
*/
typedef struct station{
    char * name;                    //Guarda el nombre de la estacion 
    size_t nameLen;                 //La longitud del nombre de la estacion 
    size_t stationId;               //El ID de la estacion 
    size_t tripsByMembers;          //Guarda la cant total de viajes hechas por miembros 
    size_t monthsVec[MONTHS];       //Vector que guarda la cant de viajes realizados en cada mes.
}tStation;


typedef enum {UNORDERED=0, ID, NAME, TRIPS} tOrder;

/*
    Estructura de los iteradores utilizados en los queries
*/
typedef struct iterators{
    size_t q1_i;                    //Iterador para el Query 1
    size_t q2_i;                    //Iterador Query 2 
    size_t q2_j;                    //Segundo Iterador Query 2 
    size_t q3_i;                    //Iterador Query 3
}tIterators;


/*
    Estructura del CDT 
*/
typedef struct bikeRentingCDT{
    tStation * vecStations;            //Vector de estructuras tipo tStation en donde cada "casillero" representa una estacion. 
    size_t stationQty;                 //Representa la cantitad total de estaciones

    tRows * matrix;                    // Una matriz dinamica que guarda los viajes realizados entre estaciones. 
    size_t oldSizeOfmatrix;            // El tamaño de la matriz 
    
    size_t firstRead;                  //Actua como un flag para saber si ya se llamo alguna vez a process data despues de haber agregado una estacion 
    tOrder order;                      //Identificador del  orden actual de la Matriz y el Vector.  
    tIterators iterators;              //La estructura que guarda los iteradores para realizar los queries. 

}tBikeRentingCDT;

/*
    Funcion de comparacion utilizada en Qsort para ordenar segun la cantidad de viajes realizados por miembros
*/
static int compare_trips(const void * a, const void * b);
/*
    Ambas funciones se utilizan en el Qsort para ordenar la matriz alfabeticamente por el nombre de la estacion. 
*/
static int compare_cols(const void *a, const void *b);
static int compare_matrix(const void *a, const void *b) ;

/*
    Funcion de comparacion utilizada en Qsort para ordernar al vector de estaciones por orden alfabetico. 
*/
static int compare_stations(const void * a, const void * b);

/*
    Funcion de comparacion usada en Qsort para ordenar el vector segun su numero de ID 
*/
static int compare_ids(const void * a, const void * b);

/*
    Funcion que utiliza qSort y las funciones de comparacion previamente explicadas para ordernar la matriz y el vector de forma Alfabetica.
*/
static void sortByName(bikeRentingADT ADT);
/*
    Utiliza a Qsort y a la funcion de comparacion compare_Trips para ordenar el vector por la cantidad de viajes realizados por miembros. 
*/
static void sortByTrips(bikeRentingADT ADT);

/*
    Recorre el vector de estaciones y se fija si el id enviado ya se encuentra en el vector, en caso de ser verdadero, devuelve 1. 
    Si no se encuentra, devuelve -1. 
*/
static int searchRepeatedId(tStation * vec,size_t size, size_t id);

/*
    Se encarga de inicializar la matriz en 0 y NULL. 
*/
static void  fillMatrixWithzeros(tRows * matrix, size_t dim,size_t * oldSize);

/*
    Agranada las columnas de la matriz dependiendo de la cantidad de estaciones agregadas e inicializa esos espacioes en 0 y NULL. 
*/
static int matrixColsAlloc(tRows * matrix, size_t dim,size_t * oldSize);
/*
    Funcion que usa una busqueda binaria para buscar el indice en el cual se encuentra el ID enviado. 
*/
static int  binarySearch(tStation * vec, int min, int max, size_t Id);

/*
    Inicializa el vector de meses en 0. 
*/
static void startMonthsInZero(size_t months[MONTHS]);


static int compare_trips(const void * a, const void * b){
    int c;
    const tStation * station1 = (const tStation *) a;
    const tStation * station2 = (const tStation *) b;
    c = station2->tripsByMembers - station1->tripsByMembers;
    if(c == 0){
    return strcasecmp(station1->name,station2->name); //En caso de que los viajes por miembros sean iguales, determino el orden alfabeticamente, utilizando strcasecmp.
    }
    return c; 
}
static void sortByTrips(bikeRentingADT ADT){
    qsort(ADT->vecStations,ADT->stationQty,sizeof(tStation),compare_trips); 
    ADT->order = TRIPS; //Aclara que el vector se encuentra ordenado segun la cantidad de viajes realizados por miembros. 
}


static int searchRepeatedId(tStation * vec,size_t size, size_t id){
    for( size_t i = 0; i < size; i++){
        if( vec[i].stationId == id){
            return 1;                   //Si halla que el ID enviado ya se encuentra en el vector, devuelve 1. 
        }
    }
    return -1; //Devuelve -1 si no se encontro ese ID en el vector de estaciones. 
}


static void  fillMatrixWithzeros(tRows * matrix, size_t dim,size_t * oldSize){
    tCols aux = {NULL,0,0,0};                       // Inicializa los campos de la matriz en 0 y NULL. 
    for (size_t i = 0; i < *oldSize;i++){
        for (size_t j = *oldSize; j < dim;j++){
            matrix[i].cols[j] = matrix[j].cols[i] = aux; // A traves del ciclo va inicializando todos los "casilleros" 
        }
    }
    
    for(size_t i=*oldSize;i<dim;i++){
        for(size_t j=*oldSize;j<dim;j++){
            matrix[i].cols[j] = aux;        //Algoritmo que rellena los nuevos lugares con 0, en caso de que exista, no pisa la informacion anterior. 
        }
    }
}

static int matrixColsAlloc(tRows * matrix, size_t dim,size_t * oldSize){
    errno = 0;

    tCols * auxDestino;
        for( size_t i=0; i < dim; i++){
            auxDestino=realloc(matrix[i].cols,dim * sizeof(tCols));
            if(auxDestino == NULL || errno == ENOMEM){                // si me quedo sin memoria libero lo ya reallocado.
                for(int j=0; j < i;j++){
                    free(matrix[j].cols);
                } 
                return 0;
            }
            matrix[i].cols=auxDestino; //Le genero el espacio a la matriz. 
        }
    fillMatrixWithzeros(matrix,dim,oldSize);
    *oldSize=dim; //Actualizo la dimension de la matriz. 
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
    return -1; //En caso de no encontrar el ID enviado, retorna -1. 
}

static void startMonthsInZero(size_t months[MONTHS]){
    for( size_t i = 0; i < MONTHS; i++){
        months[i] = 0;                    //Ciclo que se encarga de inicializar todos los espacion del vector months en 0. 
    }
}

static int compare_ids(const void * a, const void * b){
    
    const tStation * station1 = (const tStation *) a;
    const tStation * station2 = (const tStation *) b;
    return (station1->stationId-station2->stationId);
}

static void sortByName(bikeRentingADT ADT){
    qsort(ADT->vecStations,ADT->stationQty,sizeof(tStation),compare_stations); //Ordena el vector por orden alfabetico
    qsort(ADT->matrix,ADT->oldSizeOfmatrix,sizeof(tRows),compare_matrix); //Ordena las filas de la matriz por orden alfabetico
       for( size_t i = 0; i < ADT->oldSizeOfmatrix; i++){
        qsort(ADT->matrix[i].cols,ADT->oldSizeOfmatrix,sizeof(tCols),compare_cols);  //Ordena las columnas de la matriz por orden alfabetico
        }
    ADT->order = NAME; //Aclara que ahora tanto la matriz como el vector de estaciones se encuentran ordenado en orden alfabetico. 
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
    if(station1->name == NULL){
        return 1;
    }
    if(station2->name == NULL){
        return -1;
    }
    return strcasecmp(station1->name,station2->name);
}


void orderByids(bikeRentingADT ADT){
    qsort(ADT->vecStations,ADT->stationQty,sizeof(tStation),compare_ids);       //Ordena el vector de estaciones por ID
    ADT->order = ID; //Informa que ahora el vector de estaciones esta ordenado por ID. 
}

bikeRentingADT newBikesRenting(void){
    errno = 0;
   return calloc(1, sizeof(tBikeRentingCDT)); //Inicializa a traves de un calloc al ADT que sera creado en el main. 
}

size_t getNumberOfStations(bikeRentingADT ADT){
    return ADT->stationQty; //Devuelve el numero total de estaciones. 
}


int addStation(bikeRentingADT ADT, char *name,size_t id) {
    errno = 0;
    if(name == NULL){
        return 1;
    }
    if(searchRepeatedId(ADT->vecStations,ADT->stationQty,id) == -1){   // si no encuentra la estacion la agrega       
        size_t nameLenAux = strlen(name);
        char * nameAux = malloc(nameLenAux + 1);
        if(nameAux == NULL || errno == ENOMEM){
            return -1;
        }
        strcpy(nameAux, name);
        
        tStation * aux =  realloc(ADT->vecStations,(ADT->stationQty+1) * sizeof(tStation));
        if(aux == NULL || errno == ENOMEM){
            free(nameAux);
            return -1;
        }

        //Cuando ya se hizo el realloc del vector, igualamos los campos primeros, y despues el vector a aux
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
        //Para hacer el binarySearch necesito que este ordenado el vector de estaciones por ID, asi que lo ordeno de esa manera si no lo esta 
         orderByids(ADT);
    }
    int idxStart = binarySearch(ADT->vecStations,0,ADT->stationQty-1,idStart); //Guarda en idxStart el indice del vector en donde se encuentra el ID enviado (idStart).
    int idxEnd  = binarySearch(ADT->vecStations,0,ADT->stationQty-1,idEnd); //Guarda en idxEnd el indice del vector en donde se encuentra el ID enviado (idEnd). 
    //Si alguno de las dos estaciones no existe, o si el parametro de months es incorrecto, no agregamos el viaje.
    if( idxEnd == -1 || idxStart == -1 || month < 1 || month > 12) {
        return 1;
    }

    if( ADT->firstRead == 1){ //Entra a este IF siempre que es la primera vez que se llama a processData despues de haber llamado a addStation 
        tRows * aux;
        aux = realloc(ADT->matrix,ADT->stationQty * sizeof(tRows));
        if(aux == NULL || errno == ENOMEM){                         // si me quedo sin memoria, el main puede decidir liberar los recursos con freeTad o no. No es necesario liberar ahora
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
        ADT->firstRead = 0; // Apago el flag
    }
 
    if(isMember){
        ADT->vecStations[idxStart].tripsByMembers++; 
    }
    
    ADT->vecStations[idxStart].monthsVec[month-1]++;;

    if( idxEnd != idxStart){ //Solo agrego el viaje a la matrix si no es un viaje circular
        char * nameEnd = ADT->vecStations[idxEnd].name;
        char * nameStart = ADT->vecStations[idxStart].name;
        if( ADT->matrix[idxStart].cols[idxEnd].tripsTo==0 && ADT->matrix[idxStart].cols[idxEnd].tripsFrom == 0){
            
            //Lo guardo dentro de la matrix, para ver las llegadas
            ADT->matrix[idxStart].cols[idxEnd].name=nameEnd;
            ADT->matrix[idxStart].cols[idxEnd].nameLen=ADT->vecStations[idxEnd].nameLen;

            ADT->matrix[idxEnd].cols[idxStart].nameLen=ADT->vecStations[idxStart].nameLen;
            ADT->matrix[idxEnd].cols[idxStart].name = nameStart;
            //Lo guardamos en las "filas", para cuando itero en la estacion A (A--> todas las que llega)
            ADT->matrix[idxStart].name = nameStart;
            ADT->matrix[idxEnd].name = nameEnd;
            ADT->matrix[idxStart].nameLen = ADT->vecStations[idxStart].nameLen;
            ADT->matrix[idxEnd].nameLen = ADT->vecStations[idxEnd].nameLen;
        }
        ADT->matrix[idxStart].cols[idxEnd].tripsTo++;
        ADT->matrix[idxEnd].cols[idxStart].tripsFrom++;
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
    return 0;//ADT->iterators.q2_i >= ADT->oldSizeOfmatrix;
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
    if(!hasNextStartQ2(ADT) || !hasNextDestinationQ2(ADT)){
        return -1; //devuelvo basura
    }
    return ADT->matrix[ADT->iterators.q2_i].cols[ADT->iterators.q2_j].tripsTo;
}
size_t getTripsFromQ2(bikeRentingADT ADT){
    if(!hasNextStartQ2(ADT) || !hasNextDestinationQ2(ADT)){
        return -1;//devuelvo basura
    }
    return ADT->matrix[ADT->iterators.q2_i].cols[ADT->iterators.q2_j].tripsFrom;
}

char * getNameOfDestinationQ2(bikeRentingADT ADT){
    if(!hasNextStartQ2(ADT) || !hasNextDestinationQ2(ADT)){
        return NULL;
    }
    char * aux = malloc(ADT->matrix[ADT->iterators.q2_i].cols[ADT->iterators.q2_j].nameLen + 1);
    if(aux == NULL || errno == ENOMEM){
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
        return NULL;
    }
    return strcpy(aux, ADT->vecStations[ADT->iterators.q3_i].name);
}

void nextQ3(bikeRentingADT ADT){
    ADT->iterators.q3_i++;
}


void getTripsByMonthQ3(bikeRentingADT ADT,size_t tripsVec[]){
    if(!hasNextQ3(ADT)){
        return ;
    }
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

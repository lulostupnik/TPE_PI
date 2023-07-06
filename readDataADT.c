#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "errores.h"
#include "readDataADT.h"

#define BLOCK_COLUMN 5
#define BLOCK_STR 20
#define colsTocopystations 2
#define colstocopyBikes 4


typedef struct data{
    char * string;
    size_t len;
}tData;

typedef struct nodo {
    size_t id;
    char **heads;
    size_t * len;
    struct nodo *tail;
} nodo;

typedef struct nodo * list;

typedef struct readDataCDT {
    list first;
    list iter;
    tData * vec;
    size_t rows;
    size_t cols;
} readDataCDT;


static char *copyString(char *s, int *flag,size_t * len);
static char *copyRow(FILE *fp, size_t maxleght, int *flag);
static void assignId(readDataADT station);
static list addToTheListRec(char **row, list lista, int *flag, int *added,size_t cols,size_t * len);
static void freeRow( char **row, size_t cols);
static void separeteRowStations(char *row, size_t maxLenght, size_t stationNameCol,size_t stationIdCol, int *flag, readDataADT data);
static void getDatastations(readDataADT data, FILE *fp, size_t stationNameCol, size_t stationIdCol, size_t maxLenght, int *flag);
void freeRowBike(tData * data,int j);
static void separeteRowBikes(char *row, size_t maxLenght, size_t bikeIdDepar, size_t bikeDate,size_t bikeIdArr, size_t isMember, int *flag, readDataADT data);
static void getDataBikes(readDataADT data, FILE *fp, size_t bikeIdDepar, size_t bikeDate, size_t bikeIdArr,size_t isMember ,size_t maxLenght, int *flag);


//Funciones de interaccion con el usuario:

//Para el excel de Bikes:
readDataADT newReadBikes(char *tablePath, size_t bikeIdDepar, size_t bikeDate, size_t bikeIdArr,size_t isMember ,size_t maxLenght, int *flag) {
    readDataADT ans = calloc(1, sizeof(readDataCDT));
    if (ans == NULL) {
        *flag = NO_MEMORY;
        return NULL;
    }
    FILE *fp = fopen(tablePath, "r");
    if (fp == NULL) {
        free(ans);
        *flag = INCORRECT_PATH;
        return NULL;
    }
    ans->cols = colstocopyBikes;
    getDataBikes(ans, fp, bikeIdDepar, bikeDate, bikeIdArr, isMember, maxLenght, flag);
   
    fclose(fp);  
    return ans;
}
char * getDataFromPosBike(readDataADT data, size_t row, colsBikes col ,int * flag){
    *flag = OK;
    if(data == NULL){
        *flag = NULL_POINTER;
        return NULL;
    }
    if(row >= data->rows || col >= data->cols){
        *flag = INVALID_POSITION;
        return NULL;
    }
  
    char * ans = malloc(data->vec[row*colstocopyBikes + col].len+1);
    if(ans == NULL){
        *flag = NO_MEMORY;
        return NULL;
    }
    return strcpy(ans, data->vec[row*colstocopyBikes + col].string);
}


//Para el excel de station
readDataADT newReadstation(char *tablePath, size_t stationNameCol, size_t stationIdCol, size_t maxLenght, int *flag) {
    readDataADT ans = calloc(1, sizeof(readDataCDT));
    printf("%ld\n", sizeof(readDataCDT));
    if (ans == NULL) {
        *flag = NO_MEMORY;
        return NULL;
    }
    FILE *fp = fopen(tablePath, "r");
    if (fp == NULL) {
        free(ans);
        *flag = INCORRECT_PATH;
        return NULL;
    }
    getDatastations(ans, fp, stationNameCol, stationIdCol, maxLenght, flag);
    assignId(ans);
    ans->iter = ans->first;
    fclose(fp);  
    return ans;
}

int hasNext(readDataADT data) {
    return data->iter != NULL;
}
void toBegin(readDataADT data) {
    data->iter = data->first;
}

//Para ambos
size_t getrows(readDataADT data){
    return data->rows;
}
size_t getcols(readDataADT data){
    return data->cols;
}
void freeExcel(readDataADT data) {
    list aux=data->first;
    while(aux != NULL){
        list aux2=aux->tail;
        freeRow(aux->heads,data->cols);
        free(aux->len);
        free(aux);
        aux=aux2;
    }
    if(data->vec != NULL){
        for(int i=0; i<data->rows; i++){
            for(int j=0; j<data->cols;j++){
                free(data->vec[i* colstocopyBikes + j].string);
            }}
        free(data->vec);
    }
    free(data);
}

//Misc
static char *copyString(char *s, int *flag,size_t * len) {
    char *rta = NULL;
    int i;
    for (i = 0; s[i] != '\n' && s[i] != '\0'; i++) {
        if (i % BLOCK_STR == 0) {
            rta = realloc(rta, i + BLOCK_STR);
            if (rta == NULL) {
                *flag = NO_MEMORY;
                return NULL;
            }
        }
        rta[i] = s[i];
    }
    rta = realloc(rta, i + 1);
    rta[i] = 0;
    *len=i;
    return rta;
}

//Funciones que se utilizan tanto para el procesamiento de los archivos de stations, como de bikes.
static char *copyRow(FILE *fp, size_t maxleght, int *flag) {
    char *aux = malloc(maxleght);
    if (aux == NULL) {
        *flag = NO_MEMORY;
        return NULL;
    }
     char *ans = fgets(aux, maxleght, fp);
    if (ans == NULL) {
        free(aux);  
        *flag = READ_ERROR;
        return NULL;
    }
    return ans;
}



//Funciones estaticas para el proceso de los archivos de estaciones:
static void assignId(readDataADT station) {
    list aux = station->first;
    int i = 0;
    while (aux != NULL) {
        aux->id = i++;
        aux = aux->tail;
    }
}
static list addToTheListRec(char **row, list lista, int *flag, int *added,size_t cols,size_t * len) {
    char c;
    if (lista == NULL || (c = strcmp(lista->heads[0], row[0])) > 0) {
        list aux = malloc(sizeof(nodo));
        if (aux == NULL) {
            *flag = NO_MEMORY;
            return NULL;
        }
        aux->heads = row;
        aux->tail = lista;
        aux->len=len;
        *added = 1;
        return aux;
    }
    if (c == 0) {
        freeRow(row,cols);
        free(len);
        return lista;
    }
    lista->tail = addToTheListRec(row, lista->tail, flag, added,cols,len);
    return lista;
}
static void freeRow( char **row, size_t cols){
    for(int i=0;i<cols;i++)
    free(row[i]);
    free (row);
}
static void separeteRowStations(char *row, size_t maxLenght, size_t stationNameCol,size_t stationIdCol, int *flag, readDataADT data) {
    char *token;
    int added = 0;
    char **aux = malloc(colsTocopystations * sizeof(char *));
    if (aux == NULL) {
        *flag = NO_MEMORY;
        return;
    }
    int i = 0;
    int j = 0;
    *flag = OK;
    size_t * len=malloc(colsTocopystations*sizeof(size_t));
    token = strtok(row, ";");
    while (token != NULL) {
        if (j == stationNameCol) {
         aux[0] = copyString(token, flag,&len[0]);
        }else if(j == stationIdCol){
        aux[1] = copyString(token, flag,&len[1]);
        }
        if (*flag == NO_MEMORY) {
                freeRow(aux,i+1);
                return;
        }
        
        j++;
        token = strtok(NULL, ";");
    }
    data->cols = colsTocopystations;
    data->first = addToTheListRec(aux, data->first, flag, &added,data->cols,len);
    data->rows += added;
    
}
static void getDatastations(readDataADT data, FILE *fp, size_t stationNameCol, size_t stationIdCol, size_t maxLenght, int *flag) {
    char *aux;
    while ((aux = copyRow(fp, maxLenght, flag)) != NULL) {
        separeteRowStations(aux, maxLenght ,stationNameCol, stationIdCol, flag, data);
        free(aux);  
    }
    free(aux);
}
//Funciones estaticas para el proceso de los archivos de bicicletas:

void freeRowBike(tData * data,int j){
    for(int i=0; i<=j; i++){
        free(data[i].string);
    }
    free(data);
}
static void separeteRowBikes(char *row, size_t maxLenght, size_t bikeIdDepar, size_t bikeDate,size_t bikeIdArr, size_t isMember, int *flag, readDataADT data) {
    char *token;
    tData * dataAux = realloc(data->vec, sizeof(*data->vec) * (data->rows+1)*colstocopyBikes);
    if(dataAux == NULL){
        *flag = NO_MEMORY;
        return;
    }
    data->vec = dataAux;

    int j = 0;
    *flag = OK;
    token = strtok(row, ";");
    while (token != NULL) {
            if(j==bikeIdDepar){
                data->vec[data->rows*colstocopyBikes].string = copyString(token, flag ,&data->vec[data->rows*colstocopyBikes].len);
            }else if(j == bikeIdArr){
                 data->vec[data->rows*colstocopyBikes+1].string = copyString(token, flag,&data->vec[data->rows*colstocopyBikes+1].len);
            }else if(j == bikeDate){
                data->vec[data->rows*colstocopyBikes+2].string = copyString(token, flag,&data->vec[data->rows*colstocopyBikes+2].len);
            }else if(j == isMember){
                data->vec[data->rows*colstocopyBikes+3].string = copyString(token, flag,&data->vec[data->rows*colstocopyBikes+3].len);
            }
            if (*flag == NO_MEMORY) {
                freeRowBike(data->vec,j + data->rows * colstocopyBikes);
                return;
        }
        j++;
        token = strtok(NULL, ";");
    }
    data->rows++;
}
static void getDataBikes(readDataADT data, FILE *fp, size_t bikeIdDepar, size_t bikeDate, size_t bikeIdArr,size_t isMember ,size_t maxLenght, int *flag) {
    char *aux;
    while ((aux = copyRow(fp, maxLenght, flag)) != NULL) {
        separeteRowBikes(aux, maxLenght, bikeIdDepar, bikeDate, bikeIdArr, isMember, flag, data);
        free(aux);  
    }

    
}





//Funciones a borrar:
void printStation(readDataADT data) {
    int flag=OK;
    list aux=data->first;
    while (aux!=NULL){
        for(int j=0;j < data->cols;j++){
            printf("%-25s\t",aux->heads[j]);
            printf("%-25ld\t",aux->len[j]);
        }
        putchar('\n');
        
        aux=aux->tail;
        
    }
}


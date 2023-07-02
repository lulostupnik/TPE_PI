#include "readExcelTablesADT.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>


typedef struct excelData{
    char * string;
    size_t len;
}tData;

struct readExcelTablesCDT{
    tData * dataVec;
    size_t cols;
    size_t rows;
};

static void * reallocMem(void * target, size_t spaces, int * flag);
static char * getField(FILE * fp, char * limits, char * foundLimit, size_t * len, int * flag);
static tData * getData(FILE * fp, char * limits, size_t * rows, size_t * cols ,int * flag);

void freeExcelData(readExcelTablesADT data){
    if(data == NULL){
        return;
    }
    if(data->dataVec != NULL){
        for(int i=0; i<data->rows;i++){
            for(int j=0; j<data->cols;j++){
                free(data->dataVec[i*data->cols+j].string);
            }
        }
        free(data->dataVec);
    }
    free(data);
}

readExcelTablesADT newRead(char * tablePath, int * flag){
    readExcelTablesADT ans = reallocMem(NULL, sizeof(*ans), flag);
   
    if(*flag == ENOMEM){
        return NULL;
    }
    FILE * fp = fopen(tablePath, "r");
    if(fp == NULL){
        free(ans);
        *flag = INCORRECT_PATH;
        fprintf(stderr, "Invalid path for archive. \n"); // hay un errno para INCORRECT path? Vale la pena el fprintf?
        return NULL;
    }
    char limits[] = {';', '\n', EOF, 0}; // El 0 es para usar STRCHR y hacer que limites sea 0 terminated. No es para usar como limite (igual en el excel no se termina nunca con 0)
    ans->dataVec = getData(fp, limits, &ans->rows, &ans->cols, flag);
    fclose(fp);
    if(*flag == ENOMEM){
        free(ans);
        return NULL;
    }
    return ans;
}


//hay que hacer validacion aca, o queda en el user?
size_t getDimRowsExcel(readExcelTablesADT data){
    return data->rows;
}
size_t getDimColsExcel(readExcelTablesADT data){
    return data->cols;
}



char * getDataFromPos(readExcelTablesADT data, size_t row, size_t col, int * flag){
    if (row >= data->rows || col >= data->cols) {
        *flag = INVALID_POSITION;
        return NULL;
    }

    char * ans = reallocMem(NULL, data->dataVec[row*data->cols+col].len+1, flag);
    if(ans == NULL){
        *flag = ENOMEM;
        return NULL;
    }
    *flag = 0;
    return strcpy(ans, data->dataVec[row*data->cols+col].string);
}






#define BLOCK_STR 20
#define BLOCK_COLS 5
//No checkeo por ENOMEM. La funcion devulve una pseudo matrix con los datos. 
static tData * getData(FILE * fp, char * limits, size_t * rows, size_t * cols ,int * flag){
      char limit = '0';
      tData * rta = NULL;
      int cant_cols = 0;
      for(cant_cols = 0; limit != '\n' && limit != EOF; cant_cols++){
            if(cant_cols % BLOCK_COLS == 0){
                  rta = reallocMem(rta, (cant_cols+BLOCK_COLS) * sizeof(*rta), flag);
            }
            rta[cant_cols].string = getField(fp, limits, &limit, &rta[cant_cols].len , flag);
      }
      rta = reallocMem(rta, cant_cols * sizeof(*rta), flag);
      *cols = cant_cols;
      int i;
      for(i=1; limit != EOF ;i++){
            rta = reallocMem(rta, (cant_cols*(i+1))*sizeof(*rta), flag);
            for(int j = 0; j<cant_cols;j++){
                  rta[cant_cols*i+j].string = getField(fp, limits, &limit, &rta[cant_cols*i+j].len, flag);
            }
      }
      *rows = i;
      return rta;
}

static void * reallocMem(void * target, size_t spaces, int * flag){
      void * ptr = NULL;
      ptr = realloc(target, spaces);
      if(ptr == NULL){
            free(target);
            *flag = ENOMEM;
            return NULL;
      }
     
      *flag = 0;
      return ptr;
}

static char * getField(FILE * fp, char * limits, char * foundLimit, size_t * len, int * flag){
      char * rta = NULL, * search, c;
      int i;

      for(i=0, c=fgetc(fp); (search = strchr(limits, c)) == NULL; c=fgetc(fp), i++){
            if(i%BLOCK_STR == 0){
               rta = reallocMem(rta, i+BLOCK_STR, flag);
               if(*flag == ENOMEM){
                  return NULL;
               }
            }
            rta[i] = c;
      }
      rta = reallocMem(rta, 1+i, flag);
      *len = i;
      if(*flag == ENOMEM){
            return NULL;
      } 
      rta[i] = '\0';
      *foundLimit = *search;
      return rta;
}


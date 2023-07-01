#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

char * getField(FILE * fp, char * limits, char * foundLimit, int * flag);

int main(void)
{
   FILE *fp;

   fp = fopen("/mnt/c/Users/lulos/Desktop/FINAL PI/test.csv", "r");
   if (fp == NULL)
   {
      printf("No se pudo abrir el archivo\n");
      return 1;
   }
   char limits[]={';', EOF, '\n'}, foundLimit;
   int flag;
   char * line1 = getField(fp, limits,&foundLimit,&flag);
   printf(line1);

}

#define BLOCK_STR 20

/*char * getField(FILE * fp, char * limits, char * foundLimit, int * flag){
      char * rta = NULL, * search, c;
      int i;
      for(i=0, c=fgetc(fp); (search = strchr(limits, c)) == NULL; c=fgetc(fp), i++){
            if(i%BLOCK_STR == 0){
               rta = realloc(rta, BLOCK_STR + i);
               //check
            }
            rta[i] = c;
      }

      rta = realloc(rta, 1+i); // check
      rta[i] = '\0';
      *foundLimit = *search;
      return rta;
}*/



void * reallocMem(void * target, size_t spaces, int * flag){
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

char * getField(FILE * fp, char * limits, char * foundLimit, int * flag){
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
      if(*flag == ENOMEM){
            return NULL;
      } 
      rta[i] = '\0';
      *foundLimit = *search;
      return rta;
}
#ifndef _ERRORES_H
#define _ERRORES_H

typedef enum {OK = 0,NULL_POINTER, NO_MEMORY, INVALID_ID, ALREADY_ADDED, INCORRECT_PATH, INVALID_POSITION, INVALID_ARGUMENTS} tErrors;

typedef enum {ID=0, NAMES} stationColsMON; // capaz tiene que ir en otro archivo
typedef enum {START_DATE=0, EMPLACEMENT_PK_START, END_DATE, EMPLACEMENT_PK_END, IS_MEMBER} bikesColsMON;

#endif
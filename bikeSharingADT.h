#include <errno.h>
#include <stddef.h>

#ifndef BIKESHARINGADT_H
#define BIKESHARINGADT_H
typedef struct bikeRentingCDT * bikeRentingADT;

bikeRentingADT newBikesRenting();

// Libera los recursos utilizados por el ADT
void freeTad(bikeRentingADT ADT);

/*
 * Funcion para agregar estaciones:
 *
 * @param name El nombre con el que se quiere agregar la estacion
 * @param id   Id con el que se desea agregar la estacion
 * @return     Devuelve 0 si se agrego la estacion de manera correcta.
 *             Si hay errores, devuelve -1 por falta de memoria y 1 si no se agrego la estacion porque ya habia una con ese ID
 */
int addStation(bikeRentingADT ADT, char * name,size_t id);



/*
 * Funcion para agregar un viaje entre estaciones:
 *
 * @param month     Es el mes en el que se realizo el viaje (Entero entre 1 y 12, en donde 1 representa enero, 2 febrero, etc).
 * @param isMember  Parametro para indicar si el viaje lo realizo un miembro o no. Si no fue relizada por un miembro, mandar un 0. Si fue realizada por un miembro, cualquier otro numero.
 * @param idStart   Es el ID de inicio del viaje
 * @param idEnd     El ID en donde termina el viaje
 * @return          Devuelve 0 si se proceso la informacion de manera correcta. En caso de mandar parametros incorrectos (id de inicio/fin invalidos, mes invalido) devuelve 1 y no procesa la informacion. En caso de que no haya mas memoria, devulve -1 y no procesa la informacion.
 */
int processData(bikeRentingADT ADT,int month,int isMember,size_t idStart,size_t idEnd);


//Devuelve la cantidad de estaciones guardadas.
size_t getNumberOfStations(bikeRentingADT ADT);



/*
 * A continuacion hay todas funciones para poder recibir la informacion de los queries
 * Aclaraciones: antes de pedir informacion de cualquier query, hay que llamar a la funcion toBegin correspondiente.
 *               Ese llamado es valido hasta que se llama a una funcion que NO esta en el conjunto de funciones
 *               iteradoras del query correspondiente. Si se llama a una funcion que devuelve informacion de los iteradores
 *               despues de llamar a una funcion fuera del conjunto de funciones del MISMO query, la misma devolvera basura.
 *               Tambien devolvora basura si no se llama al toBegin antes de empezar.
 */


//Funciones Iteradoras Para Q1:

//Comienza el iterador para el Q1.
//Cada vez que se llame, empieza el iterador en la estacion que mas viajes por miembros tenga.
void toBeginQ1(bikeRentingADT ADT);

//Devuelve 1 si hay una proxima estacion para pedir informacion
int hasNextQ1(bikeRentingADT ADT);

//Devuelve un char * alocado en el heap del nombre de la estacion actual.
//En el caso de que no haya mas memoria o de que no hayan mas estaciones para iterar, devuelve NULL.

char * getNameQ1(bikeRentingADT ADT);

//Devuelve la cantidad de viajes hechos por miembros tiene la estacion actual.
// El el caso de que no hayan mas estaciones para iterar, devuelve basura.

size_t getTripsQ1(bikeRentingADT ADT);

//Itera a la proxima estacion.
void nextQ1(bikeRentingADT ADT);


//Funciones Iteradoras Para Q2:
//Se puede iterar en todas las estaciones (llamadas START) para llegar a todas las estaciones que haya algun viaje (sin contar los viajes circulares, de START a START), llamadas DEST
//Las funciones que devuelven char *, devuelven NULL si se pide un dato incorrecto (por ejemplo el nombre de destino, cuando no hay mas destinos) o si no hay mas memoria.
//Las funciones que devuelven size_t, devuelven basura si se pide un dato incorrecto.

//Comienza el iterador para el Q2
//Cada vez que se llame, empieza el iterador en la estacion con el nombre alfebeticamente menor.
void toBeginQ2(bikeRentingADT ADT);

//Devuelve 1 si hay una proxima estacion para iterar, 0 si no hay.
//De esta estacion, podemos ver todos los destinos a los que hay un viaje de ida o de vuelta.
int hasNextStartQ2(bikeRentingADT ADT);
//Pasa a la siguiente estacion de comienzo.
void nextStartStationQ2(bikeRentingADT ADT);

//Devuelve 1 si hay un proximo destino desde la estacion START que estoy parado. 0 si no hay.
int hasNextDestinationQ2(bikeRentingADT ADT);
//Devuelve la cantidad de viajes que hay de la estacion START a la estacion DEST.
size_t getTripsToQ2(bikeRentingADT ADT);
//Devuelve la cantidad de viajes de la estacion DEST a la estacion START
size_t getTripsFromQ2(bikeRentingADT ADT);
//Devuelve una copia del nombre de la estacion DEST
char * getNameOfDestinationQ2(bikeRentingADT ADT);
//Devuelve una copia del nombre de la estacion START
char * getNameOfStartQ2(bikeRentingADT ADT);

//Pasa a la proxima estacion de DESTINO
void nextDestinationQ2(bikeRentingADT ADT);

//Funciones Iteradoras Para Q3

//Comienza el iterador para el Q3. El mismo retornara por orden alfabetico la cantidad de viajes por mes en cada estacion/
void toBeginQ3(bikeRentingADT ADT);

//Retorna 1 si hay una proxima estacion, y 0 si no.
int hasNextQ3(bikeRentingADT ADT);

//Devuelve el nombre de la estacion actual. En caso de no haber mas estaciones, o si se queda sin memoria, devuelve basura.
char * getNameQ3(bikeRentingADT ADT);

/*
 * @param tripsVec es un array de (al menos) 12 posiciones.
 *        En el mismo se guardaran cuantos viajes hay en cada mes
 *        (En la posicion 0 la cantidad de viajes en Enero, en la pos 1 la cantidad de viajes en Febrero, etc, y en la pos 11 la cantidad de vijaes de Diciembre)
 */
void  getTripsByMonthQ3(bikeRentingADT ADT,size_t tripsVec[]);

//Itera a la proxima estacion
void nextQ3(bikeRentingADT ADT);
#endif
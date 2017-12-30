
#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include "table.h"

/*En todo momento suponemos que const void **elem es en todo momento un puntero a score, titulo, isbn, id en orden*/


typedef enum {
 ERROR = 0, OK = 1, PROBLEMAS = -1
} Status;

typedef enum {
 FALSE = 0, TRUE = 1, PROBLEMA = -1
} Bool;

typedef struct _List List;
typedef struct _Nodo Nodo;

/* Inicializa la lista y guarda el fichero donde están almacenadas las direcciones de memoria */
List* list_ini();

/* Libera la lista y todos sus elementos. */
void list_free(List* list);

/*Devuelve el número de filas de una lista*/
int list_nfilas(List *l);

/* Inserta en orden en la lista según el score que tenga (mayor a menor)*/
/* Si ya hay libros con ese score lo inserta el primero */
/*Pasamos la lista, los elementos en el orden: score, titulo, isbn, id y la posición de memoria en el fichero*/
List* list_insertInOrder(List *l, int punt, long pos);

/* Comprueba si una lista estÃ¡ vacÃ­a o no. */
Bool list_isEmpty(const List* list);

/*Devuelve la posición de memoria del nodo i-ésimo*/
long node_pos(const List * l, int i);

/*Devuelve los puntos de ese nodo*/
int node_punt(const List * l, int i);

/* Devuelve el tamaño de una lista. */
int list_size(const List* list);

/*Lee el fichero de una tabla y lo inserta en la lista*/
List *list_tableread(List *l, table_t *t);


#endif /* LIST_H */

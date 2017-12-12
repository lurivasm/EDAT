
#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>


typedef enum {
 ERROR = 0, OK = 1, PROBLEMAS = -1
} Status;

typedef enum {
 FALSE = 0, TRUE = 1, PROBLEMA = -1
} Bool;

typedef struct _List List;


/* Inicializa la lista reservando memoria e inicializa todos sus elementos. */
List* list_ini();

/* Libera la lista y todos sus elementos. */
void list_free(List* list);

/* Inserta en orden en la lista según el score que tenga */
/* Si ya hay libros con ese score lo inserta el primero */
List* list_insertInOrder(List *list, const void *pElem);

/* Extrae del principio de la lista realizando una copia del elemento almacenado en dicho nodo. */
void * list_extractFirst(List* list);

/* Extrae del final de la lista realizando una copia del elemento almacenado en dicho nodo. */
void * list_extractLast(List* list);

/* Comprueba si una lista estÃ¡ vacÃ­a o no. */
Bool list_isEmpty(const List* list);

/* Devuelve el elemento i-ésimo almacenado en la lista. En caso de error, devuelve NULL. */
const void* list_get(const List* list, int i);

/* Devuelve el tamaño de una lista. */
int list_size(const List* list);


#endif /* LIST_H */

#include "list.h"
#include "type.h"
#include "table.h"

typedef struct _Nodo {
    int punt;            /*Puntuación de un libro*/
    long pos;            /*Posición de memoria en el fichero*/
    Nodo *next;          /*Puntero a la siguiente fila, nunca reserva memoria*/
};

struct _List {
    Nodo *node;         /*Puntero al primer nodo de la lista*/
    int nfilas;         /*Número de filas*/

};


Nodo* nodo_ini(int punt, long pos){
    Nodo *n;
    n = (Nodo*)malloc(sizeof(Nodo));
    if(!n) return NULL;

    /*Metemos los datos en el nodo*/
    n->punt = punt;
    n->pos = pos;
    n->next = NULL;
    return n;
}



List* list_ini(){
    List *l;
    l = (List*)malloc(sizeof(List));
    if(!l) return NULL;
    int i;

    l->node = NULL;
    l->nfilas = 0;

    return l;
}


void nodo_free(Nodo *n){
  if(!n) return;
  free(n);
}


void list_free(List* l){
    Nodo *n;
    if(!l) return;

    if(list_isEmpty(l) == TRUE){
      free(l);
      return;
    }
    while(l->node != NULL){
        n = l->node;
        l->node = n->next;
        nodo_free(n);
    }
    free (l);
    return;
}


int list_nfilas(List *l){
  if(!l) return -1;
  return l->nfilas;
}


Bool list_isEmpty(const List* l){
    if(!l) return PROBLEMA;
    if (l->node == NULL) return TRUE;
    return FALSE;
}



List* list_insertInOrder(List *l, int punt, long pos){
    if(!l || pos == -1L || punt < 0) return NULL;
    int i;

    Nodo *n, *aux;
    n = nodo_ini(punt, pos);
    if(!n) return NULL;

    if(list_isEmpty(l) == TRUE){
        l->node = n;
        l->nfilas++;
        return l;
    }

    /*Si el primero tiene menor o igual score que el nuevo los cambiamos*/
    if((l->node)->punt <= n->punt){
        n->next = l->node;
        l->node = n;
        l->nfilas++;
        return l;
    }

   /*Resto de casos, buscamos hasta que el score de n sea mayor que el siguiente y lo ponemos el primero*/
    for(aux = l->node; aux->next != NULL; aux = aux->next){
      if((aux->next)->punt <= n->punt) break;
    }
    n->next = aux->next;
    aux->next = n;
    l->nfilas ++;
    return l;
}


long node_pos(const List * l, int i){
  if(!l || i < 0 || list_isEmpty(l)== TRUE) return -1L;
  Nodo *aux;
  int cont;

  for(cont = 0, aux = l->node; cont < i; cont++, aux = aux->next){
      if(aux == NULL) return -1L;  /*En caso de que no haya suficientes elementos en la lista*/
  }

  return aux->pos;
}


int node_punt(const List * l, int i){
  if(!l || i < 0 || list_isEmpty(l)== TRUE) return -1L;
  Nodo *aux;
  int cont;

  for(cont = 0, aux = l->node; cont < i; cont++, aux = aux->next){
      if(aux == NULL) return -1L;  /*En caso de que no haya suficientes elementos en la lista*/
  }

  return aux->punt;
}


int list_size(const List* l){
    if(!l) return -1;
    return l->nfilas;
}


List *list_tableread(List *l, table_t *t){
  if(!l || !t) return NULL;
  long pos, aux;

  pos = table_first_pos(t);
  while(pos != table_last_pos(t)){
    aux = table_read_record(t, pos);
    l = list_insertInOrder(l, table_column_get(t, 1), pos);
    if(!l)  return NULL;

    pos = aux;
  }
  return l;
}

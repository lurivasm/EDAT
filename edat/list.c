#include "list.h"
#include "type.h"

typedef struct _Nodo {
    char *isbn, *titulo;     /*ISBN y Título de un libro*/
    int id, punt;            /*Id y puntuación de un libro*/
    Nodo *next;      /*Puntero a la siguiente fila, nunca reserva memoria*/
} Nodo;

struct _List {
    Nodo *node;         /*Puntero al primer nodo de la lista*/
    int nfilas;         /*Número de filas*/
    int ncols;          /*Número de columnas de cada fila. Por defecto, son 4*/
};


Nodo* nodo_ini(){
    Nodo *n;
    n = (Nodo*)malloc(sizeof(Nodo));
    if(!n) return NULL;

    n->isbn = NULL;
    n->titulo = NULL;
    n->next = NULL;

    return n;
}



List* list_ini(){
    List *l;
    l = (List*)malloc(sizeof(List));
    if(!l) return NULL;
    int i;

    l->node = NULL;
    l->ncols = 4;
    l->nfilas = 0;

    return l;
}

void nodo_free(Nodo *n){
  if(!n) return;
  free(n->titulo);
  free(n->isbn);
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


int list_ncols(List *l){
  if(!l) return -1;
  return l->ncols;
}


int list_nfilas(List *l){
  if(!l) return -1;
  return l->nfilas;
}


type_t *list_tipos(List *l){
  if(!l) return NULL;
  return l->tipos;
}


Bool list_isEmpty(const List* l){
    if(!l) return PROBLEMA;
    if (l->node == NULL) return TRUE;
    return FALSE;
}


List* list_insertInOrder(List *l, const void **elem){
    if(!l || !elem) return NULL;
    int i;

    Nodo *n, *aux;
    n = nodo_ini();
    if(!n) return NULL;

    /*Metemos los datos en el nodo*/
  /*  n->data = (void**)malloc(sizeof(void*)*list_ncols(l));
    if(!n->data) return NULL;
    for(i = 0; i < list_ncols(l); i++){
      if(l->tipos[i] == STR){
        n->data[i] = (char*)malloc(sizeof(char)*(strlen(elem[i]) + 1));
        strcpy(n->data[i], elem[i]);
      }
      else{ n->data[i] = elem[i]; }
    }*/

    if(list_isEmpty(l) == TRUE){
        l->node = n;
        return l;
    }

    if(l->cmp_element_function(elem, l->node->data) == -1){  // Si elem es menor que el primer dato
        n->next = l->node;
        l->node = n;
        return l;
    }

   //Resto de casos
    for(aux = l->node; aux->next!=NULL && (l->cmp_element_function(aux->next->data, elem)== -1); aux = aux->next);
    n->next = aux->next;
    aux->next = n;
    l->nfilas ++;
    return l;
}

void * list_extractFirst(List* l){
    Nodo *aux, *aux2;
    void *copia;
    if(!l) return NULL;
    if(list_isEmpty(l)== TRUE) return NULL;


    if(l->node->next == NULL){     //Si sÃ³lo hay un elemento en la lista
        aux = l->node;
        l->node = NULL;
        copia = l->copy_element_function(aux->data);
        if (!copia) return NULL;

        l->destroy_element_function(aux->data);
        free(aux);
        return copia;
    }

    aux = l->node->next;
    aux2 = l->node;
    l->node = aux;

    copia = l->copy_element_function(aux2->data);
    if (!copia) return NULL;

    l->destroy_element_function(aux2->data);
    free(aux2);

    return copia;
}

void * list_extractLast(List* l){
    Nodo *aux, *aux2;
    void * copia;
    if(!l) return NULL;
    if(list_isEmpty(l)== TRUE) return NULL;

    if(l->node->next == NULL){     //Si sÃ³lo hay un elemento en la lista
        aux = l->node;
        l->node = NULL;
        copia = l->copy_element_function(aux->data);
        if (!copia) return NULL;

        l->destroy_element_function(aux->data);
        free(aux);
        return copia;
    }

    for(aux = l->node; aux->next->next != NULL; aux = aux->next);
    aux2 = aux->next;
    aux->next = NULL;

    copia = l->copy_element_function(aux2->data);
    if (!copia) return NULL;

    l->destroy_element_function(aux2->data);
    free(aux2);

    return copia;
}


const void* list_get(const List* l, int i){
    if(!l) return NULL;
    if(list_isEmpty(l)== TRUE) return NULL;
    Nodo *aux;
    int cont;

    for(cont = 1, aux = l->node; cont <= i; cont++, aux = aux->next){
        if(aux == NULL) return NULL;  //En caso de que no haya suficientes elementos en la lista
    }

    return aux->data;
}

int list_size(const List* l){
    if(!l) return -1;
    return list->num;
}

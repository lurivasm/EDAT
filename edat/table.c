#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "type.h"

struct table_ {
    FILE *ficht;         /*Fichero de los datos de la tabla*/
    int ncols;           /*Numero de columnas*/
    type_t *types;       /*Array de tipos en orden a escribir*/
    long pos;            /*Posición inicial para escribir*/
    long ultimo;         /*Posicion del último record escrito*/
    char *path;
    void **leidos;
};


void table_create(char* path, int ncols, type_t* types) {
    if(!path || !types || ncols <= 0) return;
    FILE *f;

    f= fopen(path, "w");
    if(!f) return;

    fwrite(&ncols, sizeof(int), 1, f);
    fwrite(types, sizeof(type_t), ncols, f);

    fclose(f);
}


table_t* table_open(char* path) {
    if(!path) return NULL;

    table_t *t;
    t = (table_t*)malloc(sizeof(table_t));
    if(!t) return NULL;

    t->ficht = fopen(path,"r+b");
    if(!t->ficht){
      free(t);
      return NULL;
    }

    fread(&t->ncols, sizeof(int), 1, t->ficht);

    t->types = (type_t*)malloc(sizeof(type_t)*t->ncols);
    if(!t->types){
      fclose(t->ficht);
      free(t);
      return NULL;
    }

    fread(t->types, sizeof(type_t), t->ncols, t->ficht);
    t->pos = ftell(t->ficht);

    t->path = (char*)malloc(sizeof(char)*(strlen(path)+1));
    if(!t->path) {
      table_close(t);
    }
    strcpy(t->path, path);
    t->leidos = NULL;


    return t;
}


void table_close(table_t* table) {
  if(!table) return;
  int i;


  if(table->leidos) {
    for(i = 0; i < table_ncols(table); i++){
      if(table->types[i] == STR) free(table->leidos[i]);
    }
    free(table->leidos);
  }
  free(table->path);
  free(table->types);
  fclose(table->ficht);
  free(table);
  return;
}


int table_ncols(table_t* table) {
    if(!table) return 0;
    return table->ncols;
}

char* table_data_path(table_t* table){
  if(!table) return NULL;
  return table->path;
}


type_t* table_types(table_t* table) {
    if(!table) return NULL;
    return table->types;
}


long table_first_pos(table_t* table) {
    if(!table) return -1L;
    return table->pos;
}


long table_cur_pos(table_t* table){
    if(!table) return -1L;
    return ftell(table->ficht);
}


long table_last_pos(table_t* table) {
    if(!table) return -1L;
    fseek(table->ficht, 0L, SEEK_END);
    return ftell(table->ficht);
}

/*Tomamos como origen el principio del fichero y guardamos lo leido en la estructura*/
long table_read_record(table_t* table, long pos) {
    if(!table || pos < 0 || pos > table_last_pos(table)) return -1L;
    int i, tamanio;

    type_t* tipos = table_types(table);
    if(!tipos) return -1L;

    /*Si table->leidos tiene algo lo liberamos*/
    if(table->leidos) {
      for(i = 0; i < table_ncols(table); i++){
        if(tipos[i] == STR) free(table->leidos[i]);
      }
      free(table->leidos);
    }
    table->leidos = (void**)malloc(sizeof(void*)*table_ncols(table));
    if(!table->leidos) return -1L;

    fseek(table->ficht, pos, SEEK_SET);

    for(i = 0; i < table_ncols(table); i++){
      /*Si es una cadena de caracteres, leemos primero el tmaño*/
      if (tipos[i] == STR){
        fread(&tamanio, sizeof(int), 1, table->ficht);
        table->leidos[i] = (char*)malloc(sizeof(char)*tamanio);
        if(!(table->leidos[i])){
          free(table->leidos);
          return NULL;
        }
        fread(table->leidos[i], sizeof(char)*tamanio, 1, table->ficht);
      }

      else {
      fread(&(table->leidos[i]), value_length(tipos[i], NULL), 1, table->ficht);
      }
    }

    return ftell(table->ficht);
}

/*Devolvemos un puntero de lo que haya en table->leidos*/
void * table_column_get(table_t* table, int col){
    if(!table || col <= 0 || col > table_ncols(table) || !table->leidos) return NULL;

    return  table->leidos[col-1];
}


void table_insert_record(table_t* table, void** values) {
    if(!table || !values) return;
    int i, tam;
    void **aux = values;
    type_t *tipos = table_types(table);
    if(!tipos) return;

    /*Posicionamos el ultimo lugar de la tabla*/
    if(table_last_pos(table) == -1L) return;

    /*Escribimos en el fichero*/
    for(i = 0; i < table_ncols(table); i++){
      /*En caso de que sea una cadena de caracteres escribimos primero su tamaño*/
      if(tipos[i] == STR){
        tam = value_length(STR, values[i]);
        fwrite(&tam, sizeof(int), 1, table->ficht);
        fwrite(values[i], value_length(STR, values[i]), 1, table->ficht);
      }
      else {
        fwrite(&(values[i]), value_length(tipos[i], values[i]), 1, table->ficht);
      }
    }

}

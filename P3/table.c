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

    t->ficht = fopen(path,"r");
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

    return t;
}


void table_close(table_t* table) {
  if(!table) return;

  fclose(table->ficht);
  free(table->types);
  free(table);
  return;
}


int table_ncols(table_t* table) {
    if(!table) return 0;
    return table->ncols;
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

/*Tomamos como origen el principio del fichero*/
long table_read_record(table_t* table, long pos) {
    if(!table || pos < 0 || pos > table_last_pos(table)) return -1L;
    int i, tamanio;
    type_t* tipos = table_types(table);
    if(!tipos) return -1L;

    void **values = (void**)malloc(sizeof(void*)*table_ncols(table));
    if(!values) return -1L;

    fseek(table->ficht, pos, SEEK_SET);

    for(i = 0; i < table_ncols(table); i++){
      /*Si es una cadena de caracteres, leemos primero el tmaño*/
      if (tipos[i] == STR){
        fread(tamanio, sizeof(int), 1, tabla->ficht);
        values[i] = (char*)malloc(sizeof(char)*tamanio);
        if(!(values[i])){
          free(values);
          return NULL;
        }
        fread(values[i], tamanio, 1, table->ficht);
        continue;
      }

      values[i] = (void*)malloc(sizeof(void)*sizeof(type_t));
      if(!(values[i])){
        free(values);
        return -1L;
      }
      fread(values[i], sizeof(type_t), 1, table->ficht);
    }
    for(i = 0; i < table_ncols(table); i++) free(values[i]);
    free(values);

    return ftell(table->ficht);
}


void * table_column_get(table_t* table, int col){
    if(!table || col <= 0 || col > table_ncols(table)) return NULL;
    char *datos;
    void *ret;
    type_t *tipo;
    int i, tamanio;

    /*Vemos que tipo es la columna col*/
    tipo = table_types(table);
    datos = (char*)malloc(sizeof(char));
    if(!datos) return -1L;

    /*Nos posicionamos en la columna en la que nos encontremos*/
    for(i = 0; i < col; i++){
      /*Si es una cadena de caracteres, leemos primero el tamaño*/
      if (tipo[i] == STR){
        free(datos);
        fread(tamanio, sizeof(int), 1, tabla->ficht);
        datos = (char*)malloc(sizeof(char)*tamanio);
        if(!(datos)) return NULL;

        fread(datos, tamanio, 1, table->ficht);
        if(i == col) break;
        free(datos);
        continue;
      }
      fread(datos, sizeof(char)*sizeof(type_t), 1, table->ficht);
    }
    ret = value_parse(tipo[i], datos);
    free(datos);
    free(tipo);
    return ret;
}


void table_insert_record(table_t* table, void** values) {
    if(!table || !values) return;
    int i;
    void **aux = values;

    /*Posicionamos el ultimo lugar de la tabla*/
    if(table_last_pos(table) == -1L) return;

    /*Escribimos en el fichero*/
    for(i = 0; i < table->ncols; i++){
      /*En caso de que sea una cadena de caracteres escribimos primero su tamaño*/
      if(table->types[i] == STR){
        fwrite(value_length(STR, values[i]), sizeof(int), 1, table->ficht);
        fwrite(values[i], value_length(STR, values[i]), 1, table->ficht);
        continue;
      }
      fwrite(values[i], value_length(table->types[i], values[i]), 1, table->ficht);
    }
}

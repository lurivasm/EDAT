#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "../type/type.h"

struct table_ {
    FILE *ficht;
    int ncols;
    type_t *types;
    long pos;
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

    table *t;
    t = (table*)malloc(sizeof(table));
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

    fread(t->types, sizeof(type_t), t->ncols, t->fitch);
    t->pos = ftell(t->ficht);

    return t;
}


void table_close(table_t* table) {
  if(!table) return;

  fclose(t->fitch);
  free(t->types);
  free(t);
  return;
}


int table_ncols(table_t* table) {
    if(!table) return 0;
    return t->ncols;
}


type_t* table_types(table_t* table) {
    if(!table) return NULL;
    return t->types;
}


long table_first_pos(table_t* table) {
    if(!table) return -1L;
    return t->pos;
}


long table_cur_pos(table_t* table){
    if(!table) return -1L;
    return ftell(t->fitch);
}


long table_last_pos(table_t* table) {
    if(!table) return -1L;
    fseek(t->fitch, 0L, SEEK_END);
    return ftell(t->fitch);
}


long table_read_record(table_t* table, long pos) {
    if(!table || pos < 0) return -1L;

}


void *table_column_get(table_t* table, int col){
    if(!table || col <= 0) return NULL;
}


void table_insert_record(table_t* table, void** values) {
    if(!table || !values) return;
    int i;
    void **aux = values;

    if(table_last_pos(table) == -1L) return;

    for(i = 0; i < table->ncols; i++){
      if(table->types[i] == STR){
        fwrite(value_length(STR, values[i]), sizeof(int), 1, table->ficht);
        fwrite(values[i], value_length(STR, values[i]), 1, table->ficht);
        continue;
      }
      fwrite(values[i], value_length(table->types[i], values[i]), 1, table->ficht)
    }
}

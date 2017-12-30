/* TABLE MODULE */
/* TABLE STRUCTURE TO HANDLE THE DEFINITION OF RELATIONAL TABLES */
#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "type.h"

typedef struct table_ table_t;

/* Creates the files to store an empty table */
void table_create(char* path, int ncols, type_t* types);

/* Opens a previously created table */
table_t* table_open(char* path);

/* Closes a table freeing the alloc'ed resources */
void table_close(table_t* table);

/* returns the number of columns of the table */
int table_ncols(table_t* table);

/* path to the file containing the records of the file */
char* table_data_path(table_t* table);

/* returns the data types of the columns of the table */
type_t* table_types(table_t* table);

/* returns the position of the first record of the table */
long table_first_pos(table_t* table);

/*Returns the position in the file in which the table is currently positioned*/
long table_cur_pos(table_t* table);

/* returns the last position available, where a new record should be inserted */
long table_last_pos(table_t* table);

/*
   Reads the record starting in the specified position. The record is
   read and stored in memory, but no value is returned. The value
   returned is the position of the following record in the file or -1
   if the position requested is past the end of the file.
*/
long table_read_record(table_t* table, long pos);

/*Returns a pointer to the value of the given column of the record
  currently in memory. The value is cast to a void * (it is always a
  pointer: if the column is an INT, the function will return a pointer
  to it).. Returns NULL if there is no record in memory or if the
  column doesn't exist.*/
void * table_column_get(table_t* table, int col);

/*Inserts a record in the last available position of the table. Note
   that all the values are cast to void *, and that there is no
   indication of their actual type or even of how many values we ask
   to store... why?*/
void table_insert_record(table_t* table, void** values);



#endif

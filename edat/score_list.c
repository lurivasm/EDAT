
#include "table.h"
#include "list.h"
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

int main(int argc, char **argv){
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN estado;
  SQLCHAR isbnSQL[200];

  table_t *score;
  List *lista;
  int puntos, i;
  char titulo[512], query[512], isbn[200];
  void **valores;
  long pos, aux;

  /*Comprobamos los parámetros de entrada*/
  if(argc < 3){
    printf("Error en los parámetros de entrada\n");
    printf("Introduce ./score <score> <titulo>\n");
    return EXIT_FAILURE;
  }
  puntos = atoi(argv[1]);
  if(puntos > 100){
    printf("Error en los puntos, no pueden superar 100\n");
  }
  strcpy(titulo, argv[2]);


  /*Conectamos*/
  estado = odbc_connect(&env, &dbc);
  if (!SQL_SUCCEEDED(estado)) {
      fprintf(stdout, "Error en la conexión\n");
      return EXIT_FAILURE;
  }

  /* Allocate a statement handle */
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);


  /*Comprobamos si existe el libro y guardamos su isbn*/
  sprintf(query, "SELECT isbn FROM edicion WHERE titulo = '%s';", titulo);
  SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
  SQLBindCol(stmt,1,SQL_C_CHAR, isbnSQL, sizeof(isbnSQL), NULL);
  estado = SQLFetch(stmt);
  if(!SQL_SUCCEEDED(estado)){
     fprintf(stdout,"No existe el libro con título %s\n",titulo);
     SQLCloseCursor(stmt);
     return EXIT_FAILURE;
    }
   fprintf(stdout,"El isbn del libro es: %s\n", isbnSQL);
   strcpy(isbn, isbnSQL);
   SQLCloseCursor(stmt);


  /*Abrimos la tabla*/
  score = table_open("score.dat");
  if(!score){
    printf("Error en table_open\n");
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }

  /*Inicializamos la lista*/
  lista = list_ini();
  if(!lista){
    table_close(score);
    printf("Error en list_ini\n");
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }

  /*Metemos los valores de la tabla en una lista ordenada*/
  lista = list_tableread(lista, score);
  if(!lista){
    table_close(score);
    list_free(lista);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }


  /*Metemos los valores dados en la tabla*/
  valores = (void**)malloc(sizeof(void*)*table_ncols(score));
  if(!valores){
    table_close(score);
    list_free(lista);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }
  valores[0] = puntos;
  valores[1] = titulo;
  valores[2] = isbn;

  pos = table_last_pos(score);
  table_read_record(score, table_last_pos(score));
  valores[3] = table_column_get(score, 4) + 1;



  table_insert_record(score, valores);
  lista = list_insertInOrder(lista, valores[0], pos);
  if(!lista){
    table_close(score);
    free(valores);
    list_free(lista);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }
  /*Cerramos la tabla y liberamos valores*/
  table_close(score);
  free(valores);

  /* free up statement handle */
  SQLFreeHandle(SQL_HANDLE_STMT, stmt);

  /* Desconectamos*/
  estado = odbc_disconnect(env, dbc);
  if (!SQL_SUCCEEDED(estado)) {
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}

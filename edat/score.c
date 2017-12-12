
#include "table.h"
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

int main(int agrc, char **argv){
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN estado;
  SQLCHAR isbnSQL[200];
  
  table_t *score;
  int puntos;
  char titulo[512], query[512], isbn[200];
  void **valores;

  /*Comprobamos los parámetros de entrada*/
  if(argc < 3){
    printf("Error en los parámetros de entrada\n");
    printf("Introduce ./score <score> <titulo>");
    return EXIT_FAILURE;
  }
  puntos = argv[1];
  strcpy(titulo, argv[2]);

  /*Conectamos*/
  estado = odbc_connect(&env, &dbc);
  if (!SQL_SUCCEEDED(estado)) {
      fprintf(stdout, "Error en la conexión\n");
      SQLFreeHandle(SQL_HANDLE_STMT, stmt);
      estado = odbc_disconnect(env, dbc);
      return EXIT_FAILURE;
  }

  /* Allocate a statement handle */
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

  /*Comprobamos si existe el libro y guardamos su isbn*/
  sprintf(query, "SELECT isbn FROM edicion WHERE titulo = '%s';", titulo);
  SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
  SQLBindCol(stmt,1,SQL_CHAR, isbnSQL, sizeof(isbnSQL), NULL);
  estado = SQLFetch(stmt);
  if(!SQL_SUCCEEDED(estado)){
     fprintf(stdout,"No existe el libro con título %s\n",titulo);
     SQLCloseCursor(stmt);
     return EXIT_FAILURE;
    }
   fprintf("El isbn del libro es: %s\n", isbnSQL);
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

  /*Metemos los valores dados*/
  valores = (void**)malloc(sizeof(table_ncols(score)));
  if(!valores){
    table_close(score);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }
  valores[0] = isbn;
  valores[1] = titulo;
  valores[2] = puntos;
  table_insert_record(score, valores);

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

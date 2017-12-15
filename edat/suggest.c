
#include "table.h"
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

int main(int argc, char **argv){
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN estado;
  SQLCHAR autor[200];

  int puntos, ncols;
  table_t *score;
  long pos;
  void **valores;
  char query[512];

  /*Comprobamos los parámetros de entrada*/
  if(argc < 2){
    printf("Error en los parámetros de entrada\n");
    printf("Introduce ./suggest <score>\n");
    return EXIT_FAILURE;
  }
  puntos = atoi(argv[1]);

  if(puntos > 100 || puntos < 0){
    printf("Score incorrecto\n");
    return EXIT_FAILURE;
  }

  /*Conectamos*/
  estado = odbc_connect(&env, &dbc);
  if (!SQL_SUCCEEDED(estado)) {
      fprintf(stdout, "Error en la conexión\n");
      return EXIT_FAILURE;
  }

  /* Allocate a statement handle */
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);


  /*Abrimos la tabla*/
  score = table_open("score.dat");
  if(!score){
    printf("Error en table_open\n");
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }

  /*Reservamos memoria para los valores*/
  ncols = table_ncols(score);
  if(ncols == 0){
    printf("Error en table_ncols");
    table_close(score);
    return -1;
  }
  valores = (void**)malloc(sizeof(ncols));
  if(!valores){
    printf("Error reservando memoria para valores\n");
    table_close(score);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }
  pos = table_first_pos(score);
  printf("Los libros con %d puntos son:\n", puntos);

  /*Recorremos la tabla imprimiendo los titulos cuyo score sea el indicado*/


  while(pos != table_last_pos(score)){
    pos = table_read_record(score,pos);

    valores[0] = table_column_get(score, 1);
    if(valores[0] == puntos){
      valores[1] = table_column_get(score, 2);
      printf("\tLibro: %s\t", valores[1]);
      valores[2] = table_column_get(score, 3);

      /*Buscamos el autor del libro accediendo a la base de datos libros*/
      sprintf(query, "SELECT autor FROM edicion WHERE isbn = '%s';", valores[2]);
      SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
      SQLBindCol(stmt,1,SQL_C_CHAR, autor, sizeof(autor), NULL);
      estado = SQLFetch(stmt);
      if(!SQL_SUCCEEDED(estado)){
         fprintf(stdout,"ERROR\n");
         SQLCloseCursor(stmt);
         return EXIT_FAILURE;
        }
       fprintf(stdout,"Autor: %s\n", autor);
       SQLCloseCursor(stmt);

    }

  }

  /*Cerramos todo*/
  free(valores);
  table_close(score);

  /* free up statement handle*/
  SQLFreeHandle(SQL_HANDLE_STMT, stmt);

  /* Desconectamos*/
  estado = odbc_disconnect(env, dbc);
  if (!SQL_SUCCEEDED(estado)) {
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

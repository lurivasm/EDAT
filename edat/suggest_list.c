
#include "table.h"
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "list.h"

int main(int argc, char **argv){
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN estado;
  SQLCHAR autor[200];

  int puntos, ncols, i;
  table_t *score;
  long pos, aux;
  void **valores;
  char query[512];
  List *lista;

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
    list_free(lista);
    table_close(score);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }

  printf("La lista de tamaño %d contiene estos puntos en orden:\n", list_size(lista));
  for(i = 0; i < list_size(lista); i++){
    printf("%d\t", node_punt(lista, i));
  }
  /*Buscamos los nodos de la lista cuyo score sea el introducido*/
  for(i = 0; i < list_size(lista); i++){
    if(puntos == node_punt(lista, i)) break; /*Hemos encontrado el primer libro*/
  }

  /*En caso de que busquemos en toda la lista y no esté*/
  if(i + 1 > list_size(lista)){
    printf("\nNo hay ningún libro con ese score\n");
    list_free(lista);
    free(valores);
    table_close(score);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    estado = odbc_disconnect(env, dbc);
    return EXIT_FAILURE;
  }

  printf("\nLos libros con %d puntos son:\n", puntos);

  /*En i está guardado el primer nodo con ese score*/
  /*Vamos imprimiendo el libro y el autor de los libros hasta que el score sea mayor o la lista acabe*/
  for(; i < list_size(lista) && puntos == node_punt(lista, i); i++){
    pos = node_pos(lista, i);
    table_read_record(score, pos);

    valores[1] = table_column_get(score, 2); /*Titulo del libro*/
    printf("\tLibro: %s\t", valores[1]);
    valores[2] = table_column_get(score, 3); /*ISBN del libro*/
    /*Buscamos el autor*/
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


  /*Cerramos todo*/
  free(valores);
  list_free(lista);
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

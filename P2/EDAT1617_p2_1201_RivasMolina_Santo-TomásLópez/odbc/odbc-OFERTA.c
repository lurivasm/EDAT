#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"



int main(int argc,char **argv) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN estado; /* ODBC API return status */
    SQLINTEGER id;
    int i;
    char query[512];

    fprintf(stdout, "Práctica 2\nHecha por Lucía Rivas Molina y Daniel Santo-Tomás López\n");

    /*Comprobamos los parámetros de entrada*/
    if(argc<4){
      printf("No hay suficientes parámetros\nIntroducir  <descuento> <desde> <hasta> <isbn> <isbn> .... <isbn> \n");
      return EXIT_FAILURE;
    }

    if(atoi(argv[1]) < 0 || atoi(argv[1]) > 100){
      printf("Parametros incorrectos\n");
      return EXIT_FAILURE;
    }

    estado = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(estado)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);


    /*Nos aseguramos de que existan los isbn introducidos*/
    for(i = 4;i<argc;i++){
      sprintf(query, "SELECT edicion.isbn FROM edicion WHERE edicion.isbn = '%s';", argv[i]);
      SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
      estado = SQLFetch(stmt);
      if(!SQL_SUCCEEDED(estado)){
        fprintf(stdout,"No existe el libro con ISBN %s\n",argv[i]);
        SQLCloseCursor(stmt);
        return EXIT_FAILURE;
      }
      SQLCloseCursor(stmt);
    }




    /*Seleccionamos el id más grande de la oferta para continuar con el siguiente número*/
    sprintf(query,"SELECT max(oferta.id) FROM oferta;");
    SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
    SQLBindCol(stmt,1,SQL_C_SLONG,&id,sizeof(SQLINTEGER),NULL);
    estado = SQLFetch(stmt);
    if(!SQL_SUCCEEDED(estado)){
      id = -1;
    }
    SQLCloseCursor(stmt);


    /*Insertamos los valores en la tabla*/
    for(i=4;i < argc; i++){
      sprintf(query,"INSERT INTO oferta VALUES ('%d', '%d', to_date('%s','YYYY-MM-DD'), to_date('%s','YYYY-MM-DD'), '%s');",id+1, atoi(argv[1]),argv[2] , argv[3],argv[i]);
      estado = SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
      if(!SQL_SUCCEEDED(estado)){
        SQLCloseCursor(stmt);
        return EXIT_FAILURE;
      }
      printf("Insertando oferta con id %d\n",id+1);
      SQLCloseCursor(stmt);
    }


    /*Liberamos todo*/
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);


    estado = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(estado)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

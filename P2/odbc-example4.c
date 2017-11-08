#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

/*
 * example 4 with a queries build on-the-fly, the good way
 */

int main(int argc,char **argv) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN estado; /* ODBC API return status */
    int i;

    /* CONNECT */
    if(argc<4){
      printf("No hay suficientes parámetros");
      return EXIT_FAILURE;
    }
    estado = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(estado)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    for(i = 4;i<argc;i++){
      SQLPrepare(stmt, (SQLCHAR*) "insert into Oferta values (?,?,?,?)", SQL_NTS);

      SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &argv[1] , 0, NULL);
      SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_TYPE_DATE, 0, 0, argv[2] , 0, NULL);
      SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_TYPE_DATE, 0, 0, argv[3] , 0, NULL);
      SQLBindParameter(stmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, argv[i] , 0, NULL);
      SQLExecute(stmt);
      SQLCloseCursor(stmt);
    }
    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    estado = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(estado)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

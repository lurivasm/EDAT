#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define TAM_MAX 500 /*Máximo tamaño para los char fecha e isbn*/

/*Programa que a través de un fichero pasado como argumento de entrada, llena la tabla ventas*/


int main(int argc, char **argv) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN estado;

    /*Declaramos los valores de las variables locales en c*/
    char fecha[TAM_MAX];
    char isbn[TAM_MAX];
    int num, fid;
    char query[512];
    FILE *f;

    fprintf(stdout, "Práctica 2\nHecha por Lucía Rivas Molina y Daniel Santo-Tomás López\n");

    /*Comprobamos que hay suficientes parámetros de entrada*/
    if(argc < 2){
      fprintf(stdout, "No hay suficientes parámetros de entrada\n");
      fprintf(stdout, "Recuerda meter un fichero con orden: <num_venta> <id_fidelizado> <isbn> <fecha>\n");
      fprintf(stdout, "La fecha debe estar en formato AAAA-MM-DD\n");
      return EXIT_FAILURE;
    }

    /*Inicializamos el fichero en f*/
    f = fopen(argv[1], "r");
    if(!f){
      fprintf(stdout, "Error al abrir el fichero\n");
      return EXIT_FAILURE;
    }

    /* Conectamos */
    estado = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(estado)) {
      fclose(f);
      return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*Comenzamos a sacar datos del fichero y a guardarlos en la tabla*/
    while (!feof(f)) {
        fscanf(f, "%d %d %s %s\n", &num, &fid, isbn, fecha);

        /*Comprobamos que el isbn existe en las tablas*/
        sprintf(query, "SELECT isbn FROM edicion where isbn = '%s';", isbn);
        SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        estado = SQLFetch(stmt);
        /* Loop through the rows in the result-set */
        if (!SQL_SUCCEEDED(estado)) {
            fprintf(stderr, "El isbn %s no está dentro de las tablas\n", isbn);
            SQLCloseCursor(stmt);
            continue;
        }
        SQLCloseCursor(stmt);

        /*Comprobamos que el fidelizado existe en las tablas*/
        sprintf(query, "SELECT id_fidelizado FROM fidelizados where id_fidelizado = '%d';", fid);
        SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        estado = SQLFetch(stmt);
        /* Loop through the rows in the result-set */
        if (!SQL_SUCCEEDED(estado)) {
            fprintf(stderr, "El fidelizado %d no está dentro de las tablas\n", fid);
            SQLCloseCursor(stmt);
            continue;
        }
        SQLCloseCursor(stmt);

        /*Comprobamos que la venta no está repetida*/
        sprintf(query, "SELECT edicion, num_venta FROM ventas where edicion = '%s' AND num_venta = '%d';", isbn, num);
        SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        estado = SQLFetch(stmt);
        /* Loop through the rows in the result-set */
        if (SQL_SUCCEEDED(estado)) {
            fprintf(stdout, "La venta de edición %s y número %d ya está\n", isbn, num);
            SQLCloseCursor(stmt);
            continue;
        }
        SQLCloseCursor(stmt);

        /*Si ambos están entonces pasamos a insertar los datos*/
        sprintf(query, "INSERT INTO ventas (num_venta, fidelizado, edicion, modo_pago, fecha) VALUES ('%d', '%d', '%s', 'Tarjeta', to_date('%s', 'YYYY-MM-DD'));", num, fid, isbn, fecha);
        fprintf(stdout, "Insertando venta número %d \n", num);
        estado = SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

        /* Loop through the rows in the result-set */
        if (!SQL_SUCCEEDED(estado)) {
            fprintf(stderr, "Error en Insert\n");
            SQLCloseCursor(stmt);
            fclose(f);
            return EXIT_FAILURE;
        }
        SQLCloseCursor(stmt);
    }

    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    fclose(f);
    /* DISCONNECT */
    estado = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(estado)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

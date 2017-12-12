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
    SQLINTEGER num_compra,fid,num_oferta,valor_oferta;
    SQLDOUBLE precio;
    int i;
    char query[512];


    fprintf(stdout, "Práctica 2\nHecha por Lucía Rivas Molina y Daniel Santo-Tomás López\n");

    /*Comprobamos los parámetros de entrada*/
    if(strcmp(argv[1],"add") == 0){
      if(argc<4){
        fprintf(stdout,"No hay suficientes parámetros");
        fprintf(stdout,"Introducir add <screen_name> <fecha> <isbn> <isbn>...\n o del <factura_id>\n");
        return EXIT_FAILURE;
      }
    }
    if(strcmp(argv[1],"del") == 0){
      if(argc<3){
        fprintf(stdout,"No hay suficientes parámetros");
        fprintf(stdout,"Introducir add <screen_name> <fecha> <isbn> <isbn>...\n o del <factura_id>\n");
        return EXIT_FAILURE;
      }
    }

    /* CONNECT */
    estado = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(estado)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*Implementamos primero la funcion de añadir ventas*/
    if(strcmp(argv[1],"add") == 0){
      /*Comprobamos si existen los libros a comprar*/
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




     /*Comprobamos si existe el fidelizado que compra y almacenamos su id*/
     sprintf(query, "SELECT fidelizados.id_fidelizado FROM fidelizados WHERE fidelizados.nickname = '%s';", argv[2]);
     SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
     SQLBindCol(stmt,1,SQL_C_SLONG,&fid,sizeof(SQLINTEGER),NULL);
     estado = SQLFetch(stmt);
     if(!SQL_SUCCEEDED(estado)){
       fprintf(stdout,"No existe el usuario con nickname %s\n",argv[2]);
       SQLCloseCursor(stmt);
       return EXIT_FAILURE;
     }
     printf("Id del fidelizado que compra: %d\n",fid);
     SQLCloseCursor(stmt);


     /*Si el usuario ya ha comprado en ese dia ,obtenemos su numero de factura.Sino,le asignamos uno nuevo*/
     sprintf(query,"SELECT ventas.num_venta FROM ventas WHERE ventas.fecha = '%s' AND ventas.fidelizado = '%d';",argv[3],fid);
     SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
     SQLBindCol(stmt,1,SQL_C_SLONG,&num_compra,sizeof(SQLINTEGER),NULL);
     estado = SQLFetch(stmt);
     if(!SQL_SUCCEEDED(estado)){
       SQLCloseCursor(stmt);

       sprintf(query,"SELECT max(ventas.num_venta) FROM ventas;");
       SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
       SQLBindCol(stmt,1,SQL_C_SLONG,&num_compra,sizeof(SQLINTEGER),NULL);
       estado = SQLFetch(stmt);
       if(!SQL_SUCCEEDED(estado)){
         num_compra = 100;
       }
     }
     printf("Número de factura del comprador: %d\n",num_compra+1);
     SQLCloseCursor(stmt);


     /*Insertamos cada venta con el mismo numero de factura ,fecha y fidelizado */
     for(i = 4;i<argc;i++){
       printf("Insertando el libro con isbn %s\n",argv[i]);
       sprintf(query, "INSERT INTO ventas(num_venta,fidelizado,fecha,edicion,modo_pago) VALUES ('%d', '%d', to_date('%s','YYYY-MM-DD'), '%s','Tarjeta');",num_compra+1, fid,argv[3],argv[i]);
       estado = SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
       if(!SQL_SUCCEEDED(estado)){
         SQLCloseCursor(stmt);
         return EXIT_FAILURE;
       }
       SQLCloseCursor(stmt);
     }



     /*Obtenemos el precio de cada libro*/
     /*Para cada isbn comprobamos si existen ofertas en fecha,eligiendo de las que tenga la mayor*/
     /*posteriormente,calculamos el precio final con oferta,y si no tiene,sin oferta*/
     for(i = 4;i<argc;i++){
       sprintf(query,"SELECT precio FROM edicion WHERE isbn = '%s';",argv[i]);
       SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
       SQLBindCol(stmt,1,SQL_C_DOUBLE,&precio,sizeof(SQLDOUBLE),NULL);
       estado = SQLFetch(stmt);
       if(!SQL_SUCCEEDED(estado)){
         SQLCloseCursor(stmt);
         return EXIT_FAILURE;
       }
       SQLCloseCursor(stmt);
       printf("El precio del libro %d es de %.2f\n",(i-3),precio/100.0);




       /*Obtenemos las ofertas*/
       sprintf(query,"SELECT id,descuento FROM Oferta WHERE fecha_ini < to_date('%s','YYYY-MM-DD') AND fecha_fin > to_date('%s','YYYY-MM-DD') and isbn = '%s' ORDER BY descuento DESC LIMIT 1;",argv[3],argv[3],argv[i]);
       SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
       SQLBindCol(stmt,1,SQL_C_SLONG,&num_oferta,sizeof(SQLINTEGER),NULL);
       SQLBindCol(stmt,2,SQL_C_SLONG,&valor_oferta,sizeof(SQLINTEGER),NULL);
       estado = SQLFetch(stmt);
       if(!SQL_SUCCEEDED(estado)){
         SQLCloseCursor(stmt);
        /*si no tiene ofertas,introducimos el precio normal*/
        sprintf(query,"UPDATE ventas SET precio_final =CAST('%.2f' as money)/10000 WHERE edicion = '%s';",precio,argv[i]);
        estado = SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
        if(!SQL_SUCCEEDED(estado)){
           SQLCloseCursor(stmt);
           return EXIT_FAILURE;
        }
        continue;
       }
       SQLCloseCursor(stmt);

       /*Si tiene ofertas,actualizamos el valor de precio y lo introducimos junto al número de oferta*/
       precio = precio - (valor_oferta * precio)/100;
       printf("Aplicamos al libro la oferta con id %d ,con descuento %d ,y el precio final queda como %.2f\n",num_oferta,valor_oferta,precio/100.0);
       sprintf(query,"UPDATE ventas SET precio_final = CAST('%.2f' as money)/10000 ,oferta = '%d' WHERE edicion = '%s';",precio,num_oferta,argv[i]);
       estado = SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
       if(!SQL_SUCCEEDED(estado)){
          SQLCloseCursor(stmt);
          return EXIT_FAILURE;
       }

     }



    }
    /*Implementamos el delete*/
    if(strcmp(argv[1],"del") == 0){
      /*hacemos un delete de todas las ventas asignadas a una factura*/
      sprintf(query,"DELETE FROM ventas WHERE ventas.num_venta = '%d';",atoi(argv[2]));
      estado = SQLExecDirect(stmt, (SQLCHAR*) query ,SQL_NTS);
      if(!SQL_SUCCEEDED(estado)) return EXIT_FAILURE;
      SQLCloseCursor(stmt);
      printf("Eliminado correctamente\n");
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

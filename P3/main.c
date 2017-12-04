#include "table.h"
#include "type.h"

/*Main de prueba, creamos una tabla de tres columnas*/
/*Nombre de usuario(str), nº se seguidores(int), nº seguidos(int)*/

void main(void){
  table_t *t;
  type_t *tipos;

  int ncols = 3, i;
  tipos = (type_t*)malloc(sizeof(type_t)*ncols);
  tipos[0] = STR;
  tipos[1] = INT;
  tipos[2] = INT;

  table_create("prueba.txt", 3, tipos);
  t = table_open("prueba.txt");
  if(!t){
    printf("Error en table_create\n");
  }

  printf("El número de columnas de la tabla es: %d\n", table_ncols(t));

  table_close(t);
}

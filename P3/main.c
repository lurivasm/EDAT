#include "table.h"
#include "type.h"

/*Main de prueba, creamos una tabla de tres columnas*/
/*Nombre de usuario(str), nº se seguidores(int), nº seguidos(int)*/

void main(void){
  table_t *t;
  type_t *tipos;
  void **values;
  char *nombre;
  int a, b;


  int ncols = 3, i;
  tipos = (type_t*)malloc(sizeof(type_t)*ncols);
  tipos[0] = STR;
  tipos[1] = INT;
  tipos[2] = INT;

  table_create("prueba.dat", 3, tipos);
  t = table_open("prueba.dat");
  if(!t){
    printf("Error en table_create\n");
    free(tipos);
    return;
  }

  printf("El número de columnas de la tabla es: %d\n", table_ncols(t));
  printf("El nombre del fichero es: %s\n", table_data_path(t));

  values = (void**)malloc(sizeof(void*)*ncols);
  if(!values){
    table_close(t);
    free(tipos);
    printf("Error en values\n");
    return;
  }
  nombre = (char*)malloc(sizeof(char)*(strlen("Pato")+1));
  if(!(nombre)){
    free(values);
    table_close(t);
    free(tipos);
    printf("Error en values[0]\n");
    return;
  }
  strcpy(nombre, "Pato");
  values[0] = nombre;

  values[1] = 1;

  values[2] = 2;

  printf("%s %d %d", values[0], values[1], values[2]);

  table_insert_record(t, values);

  nombre = (char*)malloc(sizeof(char)*(strlen("Potta")+1));
  if(!(nombre)){
    free(values);
    table_close(t);
    free(tipos);
    printf("Error en values[0]\n");
    return;
  }
  strcpy(nombre, "Potta");
  values[0] = nombre;
  values[1] = 3;
  values[2] = 4;
  table_insert_record(t, values);

  free(nombre);
  free(values);
  table_close(t);

  t = table_open("prueba.dat");
  if(!t) return;

  long pos;
  pos = table_read_record(t, table_first_pos(t));

  table_read_record(t, pos);
  table_close(t);
}

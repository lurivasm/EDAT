#include "table.h"
#include "type.h"

/*Main de prueba, creamos una tabla de tres columnas*/
/*Nombre de usuario(str), nº se seguidores(int), nº seguidos(int)*/

int main(void){
  table_t *t;
  type_t *tipos;
  void **values;
  char *nombre;
  int a, b;
  long pos, aux;

  int ncols = 3;
  tipos = (type_t*)malloc(sizeof(type_t)*ncols);
  tipos[0] = STR;
  tipos[1] = INT;
  tipos[2] = INT;

  table_create("prueba.dat", 3, tipos);
  t = table_open("prueba.dat");
  if(!t){
    printf("Error en table_create\n");
    free(tipos);
    return -1;
  }

  printf("El número de columnas de la tabla es: %d\n", table_ncols(t));
  printf("El nombre del fichero es: %s\n", table_data_path(t));

  values = (void**)malloc(sizeof(void*)*ncols);
  if(!values){
    free(tipos);
    table_close(t);
    printf("Error en values\n");
    return -1;
  }
  nombre = (char*)malloc(sizeof(char)*(strlen("Pato")+1));
  if(!(nombre)){
    free(values);
    free(tipos);
    table_close(t);
    printf("Error en values[0]\n");
    return -1;
  }
  strcpy(nombre, "Pato");
  values[0] = nombre;
  values[1] = 1;
  values[2] = 2;

  printf("Insertando: %s %d %d", values[0], values[1], values[2]);

  table_insert_record(t, values);

  free(nombre);

  nombre = (char*)malloc(sizeof(char)*(strlen("Pedro")+1));
  if(!(nombre)){
    free(values);
    free(tipos);
    table_close(t);
    printf("Error en values[0]\n");
    return -1;
  }

  strcpy(nombre, "Pedro");
  values[0] = nombre;
  values[1] = 3;
  values[2] = 4;

  printf("\nInsertando: %s %d %d", values[0], values[1], values[2]);
  table_insert_record(t, values);

  free(nombre);
  free(tipos);
  table_close(t);

  t = table_open("prueba.dat");
  if(!t) {
    free(values);
    return -1;
  }

  /*Leemos la primera posición*/
  pos = table_first_pos(t);
  if(pos == -1L){
    free(values);
    table_close(t);
    return -1;
  }
  aux = table_read_record(t, pos);
  if(aux == -1L){
    free(values);
    table_close(t);
    return -1;
  }

  values[0] = table_column_get(t, 1);
  values[1] = table_column_get(t, 2);
  values[2] = table_column_get(t, 3);
  printf("\nEl resultado es: %s %d %d", values[0], values[1], values[2]);

  /*Leemos la siguiente posición de memoria*/
  pos = aux;
  aux = table_read_record(t, pos);
  if(aux == -1L){
    free(values);
    table_close(t);
    return -1;
  }

  values[0] = table_column_get(t, 1);
  values[1] = table_column_get(t, 2);
  values[2] = table_column_get(t, 3);
  printf("\nEl resultado es: %s %d %d", values[0], values[1], values[2]);

  free(values);
  table_close(t);

  return 0;
}

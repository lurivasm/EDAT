
#include "table.h"

int main(int argc, char **argv){
  int puntos, ncols;
  table_t *score;
  long pos, aux;
  void **valores;

  /*Comprobamos los parámetros de entrada*/
  if(argc < 2){
    printf("Error en los parámetros de entrada\n");
    printf("Introduce ./suggest <score>");
    return -1;
  }
  puntos = argv[1];

  /*Abrimos la tabla*/
  score = table_open("score.dat");
  if(!score){
    printf("Error en table_open\n");
    return -1;
  }
  /*Reservamos memoria para los valores*/
  ncols = table_ncols(score);
  if(ncols == 0){
    printf("Error en table_ncols");
    free(valores);
    table_close(score);
    return -1;
  }
  valores = (void**)malloc(sizeof(ncols));

  pos = table_first_pos(score);

  /*Recorremos la tabla imprimiendo los titulos cuyo score sea el indicado*/
  while(pos <= table_last_pos(score)){
    aux = table_read_record(score);
    pos = aux;
    valores[0] = table_column_get(score, 1);
    if(valores[0] == puntos){
      valores[1] = table_column_get(score, 2);
      printf("Libro: %s\n", valores[1]);
    }
  }

  /*Cerramos todo*/
  free(valores);
  table_close(score);
  return 0;
}

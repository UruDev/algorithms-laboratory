/*
  Programa principal.
  Intérprete de comandos.

  Lee comandos desde la entrada estándar, invoca a funciones e imprime en la
  salida estándar.

  Con el comando de nombre "Fin" termian la ejecución.

  Los comandos de nombres "max_cantidad_intervalos" y "max_volumen_intervalos"
  invocan a las funciones 'max_cantidad' y 'max_volumen' respectivamente.
  Ambos leen los atributos de los intervalos desde la entrada estándar y tras
  obtener la solución imprime es la salida estándar o bien el valor que
  corresponde a la solución o bien el mensaje "ERROR: hay solapamiento."

  El comando de nombre "test_tiempo" prueba la eficiencia en el tiempo de
  ejecucion de la función 'max_cantidad'. Lee desde la entrada la cantidad de
  intervalos que se van a generar, el posible rango de duración de cada ellos
  y el tiempo máximo que se acepta en la ejecución de la función.
  Si no se cumple el tiempo se imprime en la salida estándar el mensaje
  "ERROR: Limite de tiempo excedido." y el tiempo que llevó la ejecución.
  En otro caso se imprime "TEST TIEMPO OK" y se procede a controlar si hay
  solapamiento y a obtener al valor que corresponde a la solución.


  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
 */

#include "include/intervalos.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <climits>
#include <time.h>

#define MAX_LONG_TXT 80

/*
  Devuelve un numero entero seudoaleatorio entre a y b
 */
uint rand(uint a, uint b) { return rand() % (b - a + 1) + a; }

/*
  Lee desde la entrada estándar 'cant_intervalos' con el formato
  inicio-fin-volumen
  Devuelve un arreglo con los intervalos leídos.
 */
intervalo_t *leer_intervalos(uint cant_intervalos) {
  intervalo_t *intervalos = new intervalo_t[cant_intervalos];
  for (uint i = 0; i < cant_intervalos; i++) {
    int res = scanf("%i - %i -%i", &intervalos[i].inicio, &intervalos[i].fin,
                    &intervalos[i].volumen);
    assert(res == 3);
    assert(intervalos[i].inicio < intervalos[i].fin);
    (void)res; // para evitar el warning al eliminar el assert
  }
  return intervalos;
}

/*
  Devuelve una copia de 'intervalos'.
 */
intervalo_t *copia_intervalos(intervalo_t *intervalos, uint cant_intervalos) {
  intervalo_t *copia = new intervalo_t[cant_intervalos];
  for (uint i = 0; i < cant_intervalos; i++)
    copia[i] = intervalos[i];
  return copia;
}

/*
  Reorganiza aleatoriamente los elementos de 'A', que tiene 'n' elementos.
 */
void shuffle(intervalo_t *A, uint n) {
  for (uint i = 0; i < n; i++) {
    uint j = i + rand(0, n - i - 1);
    intervalo_t tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
  }
}

/*
  Pre: 'cant_total'   >= 'cant_solucion' > 0.
  Pre: 'duracion_max' >= 'duracion_min'  >= 1.
  Post: Devuelve un arreglo de intervalos de tamaño 'cant_total' tal que
  el subconjunto mas grande de intervalos compatibles es de de tamaño 
  'cant_solucion'. La duracion de cada intervalo retornado esta 
  entre duracion_min' y 'duracion_max'.
 */
intervalo_t* generar_entrada(uint cant_total, uint cant_solucion, 
  uint duracion_min, uint duracion_max)  {
  intervalo_t* intervalos = new intervalo_t[cant_total];
  //Agrego un desfasaje al inicio del primer intervalo.
  uint inicio = duracion_max;
  //A continuacion genero 'cant_solucion' intervalos que no se 
  //solapen entre si. Estos seran los intervalos 'solucion'.
  for (uint i = 0; i < cant_solucion; i++) {  
    int duracion = rand(duracion_min, duracion_max);
    intervalos[i].inicio = inicio;
    intervalos[i].fin = inicio + duracion;
    inicio = intervalos[i].fin;
  }
  //A continuacion genero intervalos que se solapen con los intervalos
  //'solucion' generados en el paso anterior.
  for (uint i = cant_solucion; i < cant_total; i++) { 
    //Elijo aleatoriamente uno de los intervalos 'solucion'.
    uint k = rand(0, cant_solucion-1); 
    //Creo un nuevo intervalo que se solape con el intervalo 'solucion'
    //elegido y hago que su tiempo de fin sea posterior al tiempo de fin
    //del intervalo 'solucion'.
    uint duracion = rand(duracion_min, duracion_max);
    uint fin = rand(intervalos[k].fin, intervalos[k].fin + duracion - 1);
    intervalos[i].inicio = fin - duracion;
    intervalos[i].fin = fin;
  }
  //Desordeno el arreglo para que la solucion no tenga un orden definido.
  shuffle(intervalos, cant_total);
  return intervalos;
}

/*
  Devuelve 'true' si y solo si cada para de posiciones de 'solucion' cuyos
  valores son 'true' corresponden a identificadores en 'intervalos' que no se
  solapan. El chequeo se realiza en O(n*log(n))
 */
bool hay_solapamiento(bool *solucion, intervalo_t *intervalos, uint n) {
  uint *seleccionados = new uint[n];
  uint cant_seleccionados = 0;
  for (uint i = 0; i < n; i++) {
    if (solucion[i]) {
      seleccionados[cant_seleccionados] = i;
      cant_seleccionados++;
    }
  }
  bool solapa = false;
  for (uint i = 0; ((i < cant_seleccionados - 1) && (!solapa)); i++)
    for (uint j = i + 1; ((j < cant_seleccionados) && (!solapa)); j++)
      if ((intervalos[seleccionados[i]].fin >
           intervalos[seleccionados[j]].inicio) &&
          (intervalos[seleccionados[j]].fin >
           intervalos[seleccionados[i]].inicio))
        solapa = true;

  delete[] seleccionados;
  return solapa;
}

int main() {

  srand(12345678);
  uint cont_comandos = 0;
  char nom_comando[MAX_LONG_TXT];
  bool salir = false;
  while (!salir) {
    // mostrar el prompt
    cont_comandos++;
    printf("%u>", cont_comandos);

    // leer el comando
    scanf("%s", nom_comando);

    // procesar el comando
    if (!strcmp(nom_comando, "Fin")) {
      salir = true;
      printf("Fin.\n");

    } else if (!strcmp(nom_comando, "#")) {
      char resto_linea[MAX_LONG_TXT];
      scanf("%[^\n]", resto_linea);
      printf("#%s.\n", resto_linea);

    } else if (!strcmp(nom_comando, "max_cantidad_intervalos")) {
      uint cant_intervalos;
      scanf("%i", &cant_intervalos);
      intervalo_t *intervalos = leer_intervalos(cant_intervalos);
      intervalo_t *copia = copia_intervalos(intervalos, cant_intervalos);
      bool *solucion = max_cantidad(copia, cant_intervalos);
      bool hay = hay_solapamiento(solucion, intervalos, cant_intervalos);
      if (hay)
        printf("ERROR: hay solapamiento. \n");
      else {
        uint cant = 0;
        for (uint i = 0; i < cant_intervalos; i++)
          if (solucion[i])
            cant++;
        printf("Cantidad: %i\n", cant);
      }

      delete[] copia;
      delete[] intervalos;
      delete[] solucion;

    } else if (!strcmp(nom_comando, "max_volumen_intervalos")) {
      uint cant_intervalos;
      scanf("%i", &cant_intervalos);
      intervalo_t *intervalos = leer_intervalos(cant_intervalos);
      intervalo_t *copia = copia_intervalos(intervalos, cant_intervalos);
      bool *solucion = max_volumen(copia, cant_intervalos);
      bool hay = hay_solapamiento(solucion, intervalos, cant_intervalos);
      if (hay)
        printf("ERROR: hay solapamiento. \n");
      else {
        uint vol = 0;
        for (uint i = 0; i < cant_intervalos; i++)
          if (solucion[i])
            vol += intervalos[i].volumen;
        printf("Volumen: %i\n", vol);
      }

      delete[] copia;
      delete[] intervalos;
      delete[] solucion;

    } else if (!strcmp(nom_comando, "test_tiempo")) {
      uint cant_total, cant_solucion, duracion_min, duracion_max, timeout;
      scanf("%i - %i -%i -%i -%i", &cant_total, &cant_solucion, &duracion_min,
            &duracion_max, &timeout);
      intervalo_t *intervalos = generar_entrada(cant_total, cant_solucion,
                                                duracion_min, duracion_max);
      intervalo_t *copia = copia_intervalos(intervalos, cant_total);
      clock_t t;
      t = clock();
      bool *solucion = max_cantidad(copia, cant_total);
      t = clock() - t;
      float tiempo_transcurrido = ((float)t) / CLOCKS_PER_SEC;
      if (tiempo_transcurrido > timeout)
        fprintf(stderr, "ERROR: Limite de tiempo excedido. (%.1f segundos)\n",
                tiempo_transcurrido);
      else {
        fprintf(stderr, "TEST TIEMPO OK\n");
        uint cant = 0;
        for (uint i = 0; i < cant_total; i++)
          if (solucion[i])
            cant++;
        bool hay = hay_solapamiento(solucion, intervalos, cant_total);
        if (hay)
          printf("ERROR: hay solapamiento. \n");
        else {
          printf("Cantidad: %i\n", cant);
        }
      }

      delete[] copia;
      delete[] intervalos;
      delete[] solucion;

    } else {
      printf("Comando no reconocido.\n");
    }
    // lee el resto de la línea
    fgets(nom_comando, MAX_LONG_TXT, stdin);

  } // while
}

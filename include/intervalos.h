/*
  Módulo de definición de `intervalos'.


  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
 */

#ifndef _INTERVALOS_HPP
#define _INTERVALOS_HPP

typedef unsigned int uint;

struct intervalo_t {
  // fin > inicio
  uint inicio;
  uint fin;
  uint volumen;
};

/* Devuelve un arreglo de booleanos de 'n' con TRUE en los intervalos asignados,
   los cuales no se superponen.
   La cantidad de intervalos asignados debe ser la máxima posible.
   'intervalos' es un arreglo con 'n' intervalo_t.
   El tiempo de ejecucion de peor caso debe ser O(n*log(n)).
*/
bool *max_cantidad(const intervalo_t *intervalos, uint n);

/* Devuelve un arreglo de booleanos de 'n' con TRUE en los intervalos asignados,
   los cuales no se superponen.
   La suma de los volumenes de los intervalos asignados debe ser la máxima
   posible.
   'intervalos' es un arreglo con 'n' intervalo_t.
    El tiempo de ejecucion de peor caso debe ser O(n*log(n)).
*/
bool *max_volumen(const intervalo_t *intervalos, uint n);

#endif

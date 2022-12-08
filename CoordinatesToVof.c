/**
This small code is just an example for initializing a VoF field from a set of coordinates.
This code reads the data file generated by InitialCondition.m and use the Basilisk function [distance](http://basilisk.fr/src/distance.h).
*/

#include "axi.h"
#include "fractions.h"
#include "distance.h"
#include "navier-stokes/centered.h"

scalar f[], * interfaces = {f};

int main(int argc, char const *argv[]) {

  L0 = 8;
  origin (-L0/2, 0.0);
  init_grid (1 << (11));
  refine(x < 1.1 && x > -2.1 && y < 2.0 && level < 12);

  coord* InitialShape;

  char filename[60];
  
  // the coordinates X, Y must be space separated. 
  sprintf(filename,"Test.dat");
  FILE * fp = fopen(filename,"rb");
  if ( fp == NULL ){
    fprintf(ferr, "There is no file named %s\n", filename);
    return 1;
  }
  InitialShape = input_xy(fp);
  fclose (fp);
  scalar d[];
  distance (d, InitialShape);

  /**
  The distance function is defined at the center of each cell, we have
  to calculate the value of this function at each vertex. */
  vertex scalar phi[];
  foreach_vertex(){
    phi[] = -(d[] + d[-1] + d[0,-1] + d[-1,-1])/4.;
  }
  /**
  We can now initialize the volume fraction of the domain. */
  fractions (phi, f);
  output_facets (f);
  dump (file = "Test");
}

/*
 *  Delaunay.h
 *  
 *  Gilles Dumoulin's C++ implementation of Paul Bourke's Delaunay triangulation algorithm.
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *  Paper:  http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/index.html
 *
 */
#ifndef Delaunay_H
#define Delaunay_H

#include <iostream>
#include <stdlib.h> // for C qsort 
#include <cmath>
#include <time.h> // for random

const int MaxVertices = 500;
const int MaxTriangles = 1000;
//const int n_MaxPoints = 10; // for the test programm
const double EPSILON = 0.000001;

struct ITRIANGLE{
  int p1, p2, p3;
};

struct IEDGE{
  int p1, p2;
};

struct XYZ{
  double x, y, z;
};

int XYZCompare(const void *v1, const void *v2);
int Triangulate(int nv, XYZ pxyz[], ITRIANGLE v[], int &ntri);
int CircumCircle(double, double, double, double, double, double, double, double, double&, double&, double&);

#endif



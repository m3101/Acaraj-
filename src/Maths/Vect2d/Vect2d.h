/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#ifndef AC_VECT2d
#define AC_VECT2d
typedef struct 
{
    double i;
    double j;
}vect2d;

/*Returns the dot product between two vectors*/
double dotProduct2d(vect2d* u,vect2d* v);
/*Returns the magnitude of the vector resulting from the 3d cross product of u and v*/
double crossProductMagnitude(vect2d* u,vect2d* v);
/*Scales a vector and stores the result on w*/
void scale2d(vect2d* u,double s,vect2d* w);
/*Adds two vectors and stores the result on w*/
void add2d(vect2d* u,vect2d* v,vect2d* w);
/*Rotates a vector by *rad* radians and stores the result on v*/
void rot2d(vect2d* u,vect2d* w,double rad);
/*Normalizes a vector*/
void normalize2d(vect2d* u);
#endif
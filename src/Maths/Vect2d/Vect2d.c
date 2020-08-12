/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "Vect2d.h"
#include <math.h>
/*Returns the dot product between two vectors*/
double dotProduct2d(vect2d* u,vect2d* v)
{
    return u->i*v->i+u->j*v->j;
}
/*Returns the magnitude of the vector resulting from the 3d cross product of u and v*/
double crossProductMagnitude(vect2d* u,vect2d* v)
{
    return u->i*v->j-u->j*v->i;
}
/*Scales a vector and stores the result on w*/
void scale2d(vect2d* u,double s,vect2d* w)
{
    w->i=u->i*s;
    w->j=u->j*s;
}
/*Adds two vectors and stores the result on w*/
void add2d(vect2d* u,vect2d* v,vect2d* w)
{
    w->i=u->i+v->i;
    w->j=u->j+v->j;
}
/*Subtracts two vectors and stores the result on w*/
void sub2d(vect2d* u,vect2d* v,vect2d* w)
{
    w->i=u->i-v->i;
    w->j=u->j-v->j;
}
/*Rotates a vector by *rad* radians and stores the result on v*/
void rot2d(vect2d* u,vect2d* w,double rad)
{
    /*We store the values before rotating so that rot2d(u,u,*x*) is possible.*/
    double i=u->i,j=u->j;
    w->i=i*cos(rad)-j*sin(rad);
    w->j=i*sin(rad)+j*cos(rad);
}
/*Normalizes a vector*/
void normalize2d(vect2d* u)
{
    double magnitude=dotProduct2d(u,u);
    u->i/=magnitude;u->j/=magnitude;
}
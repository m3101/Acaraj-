#include <SDL2/SDL.h>
#include "../Maths/Vect2d/Vect2d.h"
void squigglyLine(double x0,double y0,double x1,double y1,double squigglelen,unsigned char squiggliness,SDL_Renderer* renderer)
{
    vect2d a,b,c,d,e;
    int cx0,cx1,cy0,cy1;
    char rx,ry;
    a.i=x0;a.j=y0;
    b.i=x1;b.j=y1;
    sub2d(&b,&a,&c);
    double ratio=sqrt(dotProduct2d(&c,&c))/squigglelen,scale=1;
    scale2d(&c,1/ratio,&c);
    b.i=a.i;b.j=a.j;
    while(ratio>=0)
    {
        rx=(rand()%squiggliness)-((squiggliness-1)/2);
        ry=(rand()%squiggliness)-((squiggliness-1)/2);
        if(ratio>1)
        {
            scale2d(&c,scale,&d);
            add2d(&a,&d,&e);
            e.i+=rx;
            e.j+=ry;
            cx0=b.i;cx1=e.i;
            cy0=b.j;cy1=e.j;
            SDL_RenderDrawLine(renderer,cx0,cy0,cx1,cy1);
            b.i=e.i;
            b.j=e.j;
            ratio--;
            scale++;
        }
        else
        {
            e.i=x1+rx;
            e.j=y1+ry;
            cx0=b.i;cx1=e.i;
            cy0=b.j;cy1=e.j;
            SDL_RenderDrawLine(renderer,cx0,cy0,cx1,cy1);
            break;
        }
    }
}
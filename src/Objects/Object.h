#ifndef AC_OBJECT
#include <SDL2/SDL.h>
#include "../states.h"
#define AC_OBJECT
/*
Note: if "frame" returns -1, the object is destroyed.
*/
typedef struct ac_object{
    void* objdata;
    char (*frame)(struct ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags);
    void (*destroy)(struct ac_object **self);
}ac_object;
#endif
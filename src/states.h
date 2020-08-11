#ifndef AC_STATES
#define AC_STATES
#include <SDL2/SDL.h>
typedef struct ac_state{
    void* stateVars;
    void (*init)(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags);
    void (*event)(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt);
    void (*frame)(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags);
    void (*destroy)();
}ac_state;
#endif
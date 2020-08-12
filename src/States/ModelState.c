/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"

struct {} ModelState_Vars={};
void ModelState_init(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{    
}
void ModelState_event(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt)
{    
}
void ModelState_frame(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{    
}
void ModelState_destroy()
{
}
ac_state ModelState={
    .stateVars=&ModelState_Vars,
    .init=ModelState_init,
    .event=ModelState_event,
    .frame=ModelState_frame,
    .destroy=ModelState_destroy,
    .id=0
};
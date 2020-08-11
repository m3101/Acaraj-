/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"
#include <stdio.h>

struct {} TestState_Vars={};
void TestState_init(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    printf("In test state.\n");
}
void TestState_event(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt)
{
}
void TestState_frame(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{    
}
void TestState_destroy()
{
}
ac_state TestState={
    .stateVars=&TestState_Vars,
    .init=TestState_init,
    .event=TestState_event,
    .frame=TestState_frame,
    .destroy=TestState_destroy
};
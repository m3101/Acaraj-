/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"
#include "../statelist.h"
#include "../Logic/timeCP.h"
#include "../PackedResources/resources.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

struct {} LoadState_Vars={};
uint64_t tstart;
Uint8 *buf;
Uint32 alength;
SDL_Surface* icon_s;
SDL_Texture *icon;
SDL_Rect drect={
    .x=320,
    .y=240,
    .w=200,
    .h=171
};
SDL_AudioDeviceID deviceId;
SDL_AudioSpec aspec;
const int ox=320-100,oy=240-(171/2);
void LoadState_init(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{    
    printf("- Acarajé - A game inspired by Nidhogg and Eggnogg\nCopyright (C) 2020 Amélia O. F. da S.\n");
    icon_s=IMG_Load_RW(SDL_RWFromConstMem(_binary_icon_png_start,_binary_icon_png_end-_binary_icon_png_start),1);
    icon=SDL_CreateTextureFromSurface(renderer,icon_s);
    SDL_zero(aspec);
    aspec.freq=48000;
    aspec.format=AUDIO_S16;
    aspec.channels=2;
    aspec.samples=4096;
    aspec.callback=NULL;
    alength=_binary_intro_wav_end-_binary_intro_wav_start;
    SDL_LoadWAV_RW(SDL_RWFromConstMem(_binary_intro_wav_start,alength),\
                    1,&aspec,&buf,&alength);
    deviceId = SDL_OpenAudioDevice(NULL, 0, &aspec, NULL, 0);
    SDL_QueueAudio(deviceId,buf,alength);
    SDL_PauseAudioDevice(deviceId, 0);
    tstart=system_current_time_millis();
    srand(*(unsigned int*)(&tstart+4));
}
void LoadState_event(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt)
{
}
void LoadState_frame(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    if(system_current_time_millis()-tstart<3500)
    {
        drect.x=ox+((rand()%3)-1);
        drect.y=oy+((rand()%3)-1);
        SDL_RenderCopy(renderer,icon,NULL,&drect);
    }
    if(system_current_time_millis()-tstart>4500)
        *next=&TestState;
}
void LoadState_destroy()
{
    SDL_DestroyTexture(icon);
    SDL_FreeSurface(icon_s);
    SDL_FreeWAV(buf);
    SDL_CloseAudioDevice(deviceId);
}
ac_state LoadState={
    .stateVars=&LoadState_Vars,
    .init=LoadState_init,
    .event=LoadState_event,
    .frame=LoadState_frame,
    .destroy=LoadState_destroy
};
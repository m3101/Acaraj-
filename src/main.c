#include "states.h"
#include "statelist.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main()
{
    /*Initializing everything*/
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)return -1;
    SDL_Window* mwindow = SDL_CreateWindow("Acarajé V0.0.0-prealpha",0,0,640,480,SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer;
    ac_state *cur_state=&LoadState,*next_state=NULL;
    if(mwindow == NULL)
    {
        SDL_Quit();
        return -1;
    }
    renderer = SDL_CreateRenderer(mwindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        SDL_DestroyWindow(mwindow);
        SDL_Quit();
        return -1;
    }

    /*Main loop*/
    char ac_flags=01;
    /*
    Flags:
    01 - Continue main loop (halts when unset)
    */
    uint32_t tstart,tend,frame_time=1000/30;
    cur_state->init(&cur_state,&next_state,renderer,mwindow,&ac_flags);
    SDL_Event evt;
    while(ac_flags&01)
    {
        tstart=SDL_GetTicks();
        SDL_RenderClear(renderer);
        /*Managing state changes*/
        if(next_state!=NULL)
        {
            cur_state->destroy();
            cur_state=next_state;
            next_state=NULL;
            cur_state->init(&cur_state,&next_state,renderer,mwindow,&ac_flags);
        }

        /*Event handling*/
        while(SDL_PollEvent(&evt))
        {
            if(evt.type == SDL_QUIT){ac_flags&=!01;}
            cur_state->event(&cur_state,&next_state,renderer,mwindow,&ac_flags,&evt);
        }

        cur_state->frame(&cur_state,&next_state,renderer,mwindow,&ac_flags);

        /*Rendering and framerate stabilization*/
        SDL_RenderPresent(renderer);
        tend=SDL_GetTicks();
        if(tend-tstart<frame_time)
            SDL_Delay(frame_time-(tend-tstart));
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(mwindow);
    SDL_Quit();
}
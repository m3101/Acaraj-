/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/
#include "../Logic/ll.h"
#include "AudioSys.h"
#include "../../lib/Simple-SDL2-Audio/src/audio.h"
#include <stdlib.h>

/*
Plays a random source from "audio"
*/
void ac_play_audio(ac_audio* audio)
{
    if(audio->length==0)return;
    ll*cur=audio->sources;
    unsigned char select=rand()%audio->length,i=0;
    for(;i<select;i++)cur=cur->next;
    playSoundFromMemory(((Audio*)cur->self),SDL_MIX_MAXVOLUME);
}
/*
Adds a source "src" to "audio"
*/
void ac_add_audio(Audio* src,ac_audio* audio)
{
    ll_push(&audio->sources,src);
    audio->length++;
}

void ac_free_audio(ac_audio* audio)
{
    ll* cur=audio->sources;
    if(audio->length==0)return;
    /*Go to the end of the list*/
    for(;cur->next!=NULL;cur=cur->next);
    /*Free each element*/
    for(;cur!=NULL;cur=cur->prev)
    {
        freeAudio(cur->self);
        cur->self=NULL;
    }
    ll_free(&audio->sources);
}
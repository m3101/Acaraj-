/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/
#include "musicMixer.h"
#include "../../lib/Simple-SDL2-Audio/src/audio.h"
#include <stdlib.h>

#if AC_USE_TTS
#include <espeak-ng/speak_lib.h>
char *options[]={
    "Acarajéééé",
    "Uélcom",
    "Iéa"
};
int lenopt=3;
static int Interruption(void* arg)
{
    espeak_SetVoiceByName("pt");
    espeak_Synth(options[rand()%lenopt],500,0,0,0,0,NULL,NULL);
    return 0;
}
#endif

static float randomF()
{
    float r=rand();
    return r/RAND_MAX;
}

static char nonePlaying(Audio **list[],int len)
{
    for(int i=0;i<len;i++)
        if ((*list[i])->playing)
            return 0;
    return 1;
}

void playAutomusic(Automusic *music)
{
    if(nonePlaying(music->trackList,music->tracknumber)&&!(*(music->beat))->playing)
    {
        #if AC_USE_TTS
        if(randomF()<0.5)
            SDL_CreateThread(Interruption,"Synthetiser",NULL);
        #endif
        for(int i=0;i<music->tracknumber;i++)
            if(randomF()<=music->trackProbablilities[i])
                playSoundFromMemory((*(music->trackList[i])),SDL_MIX_MAXVOLUME);
        if(randomF()<0.8)
            playSoundFromMemory(*(music->beat),SDL_MIX_MAXVOLUME);
    }
}
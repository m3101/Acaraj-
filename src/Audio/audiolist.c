/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/
#include "audiolist.h"
#include "AudioSys.h"
#include "../PackedResources/resources.h"
#include "../../lib/Simple-SDL2-Audio/src/audio.h"
void loadAudios()
{
    ac_add_audio(createAudioFromMemory(_binary_intro_wav_start,_binary_intro_wav_end,0,SDL_MIX_MAXVOLUME),&intro);
    deathsfolia=createAudioFromMemory(_binary_deathsfolia_wav_start,_binary_deathsfolia_wav_end,1,SDL_MIX_MAXVOLUME);
}
void freeAudios()
{
    ac_free_audio(&intro);
    freeAudio(deathsfolia);
}
ac_audio intro={
    .sources=NULL,
    .length=0
};
Audio* deathsfolia;
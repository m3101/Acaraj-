/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/
#include "audiolist.h"
#include "AudioSys.h"
#include "musicMixer.h"
#include "../PackedResources/resources.h"
#include "../../lib/Simple-SDL2-Audio/src/audio.h"
void loadAudios()
{
    intro = createAudioFromMemory(_binary_intro_wav_start,_binary_intro_wav_end,0,SDL_MIX_MAXVOLUME);
    full = createAudioFromMemory(_binary_acarajefull_wav_start,_binary_acarajefull_wav_end,1,SDL_MIX_MAXVOLUME);
    bass = createAudioFromMemory(_binary_1_Bass_wav_start,_binary_1_Bass_wav_end,0,SDL_MIX_MAXVOLUME);
    base = createAudioFromMemory(_binary_2_Base_wav_start,_binary_2_Base_wav_end,0,SDL_MIX_MAXVOLUME);
    arpeggio = createAudioFromMemory(_binary_3_Arpeggio_wav_start,_binary_3_Arpeggio_wav_end,0,SDL_MIX_MAXVOLUME);
    llama1 = createAudioFromMemory(_binary_4_Llama1_wav_start,_binary_4_Llama1_wav_end,0,SDL_MIX_MAXVOLUME);
    llama2 = createAudioFromMemory(_binary_5_Llama2_wav_start,_binary_5_Llama2_wav_end,0,SDL_MIX_MAXVOLUME);
    kicker = createAudioFromMemory(_binary_6_Kicker_wav_start,_binary_6_Kicker_wav_end,0,SDL_MIX_MAXVOLUME);
}
void freeAudios()
{
    freeAudio(intro);
    freeAudio(bass);
    freeAudio(base);
    freeAudio(arpeggio);
    freeAudio(llama1);
    freeAudio(llama2);
    freeAudio(kicker);
}
Audio *intro;
Audio *bass;
Audio *base;
Audio *arpeggio;
Audio *llama1;
Audio *llama2;
Audio *kicker;
Audio *full;

Audio **maintracklist[]={&bass,&base,&arpeggio,&llama1,&llama2};
float mainprobabilities[]={.9f,.5f,.3f,.2f,.4f};
Automusic mainSong={
    maintracklist,
    &kicker,
    mainprobabilities,
    5
};
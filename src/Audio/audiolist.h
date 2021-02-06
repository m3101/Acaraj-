/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/
#ifndef AC_AUDIOLIST
#define AC_AUDIOLIST
#include "AudioSys.h"
#include "musicMixer.h"
#include "../../lib/Simple-SDL2-Audio/src/audio.h"
extern void loadAudios();
extern void freeAudios();
extern Audio *intro;
extern Audio *bass;
extern Audio *base;
extern Audio *arpeggio;
extern Audio *llama1;
extern Audio *llama2;
extern Audio *kicker;
extern Audio *full;
extern Automusic mainSong;
#endif
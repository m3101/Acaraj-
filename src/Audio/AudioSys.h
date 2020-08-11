/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/
#ifndef AC_AUDIOSYS
#define AC_AUDIOSYS
#include "../Logic/ll.h"
#include "../../lib/Simple-SDL2-Audio/src/audio.h"
typedef struct {
    ll* sources;
    unsigned char length;
} ac_audio;
/*
Plays a random source from "audio"
*/
void ac_play_audio(ac_audio* audio);
/*
Adds a source "src" to "audio"
*/
void ac_add_audio(Audio* src,ac_audio* audio);
/*
Frees an audio object
*/
void ac_free_audio(ac_audio* audio);
#endif
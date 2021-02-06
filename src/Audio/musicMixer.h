/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/
#ifndef AC_MUSICMIXER
#define AC_MUSICMIXER
#include "../../lib/Simple-SDL2-Audio/src/audio.h"

typedef struct Automusic
{
    Audio ***trackList;
    Audio **beat;
    float *trackProbablilities;
    int tracknumber;
}Automusic;

void playAutomusic(Automusic* music);
#endif
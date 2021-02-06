#include <espeak-ng/speak_lib.h>

int main()
{
    espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK, 500, NULL, 0);
    espeak_SetVoiceByName("en");
    espeak_Synth("Acarajé",500,0,0,0,0,NULL,NULL);
    return 0;
}
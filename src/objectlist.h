#ifndef AC_OBJIST
#define AC_OBJLIST
#include "states.h"
#include "Objects/Object.h"

/*
Player
*/
typedef struct {
    double x;
    double y;
    char team;
    double swordAng;
}player_data;
char Player_frame(ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags);
void Player_destroy(ac_object** self);
ac_object *Player(double x, double y, char team);
#endif
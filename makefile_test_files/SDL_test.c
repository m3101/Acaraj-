#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>

int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)return -1;
    TTF_Init();
    SDL_Window* mwindow = SDL_CreateWindow("test",0,0,1,1,SDL_WINDOW_OPENGL);
    SDL_GLContext Context = SDL_GL_CreateContext(mwindow);
    return 0;
}
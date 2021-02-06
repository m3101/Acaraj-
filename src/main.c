volatile static char *copyright_message ="Acarajé - A minimalist swordfighting game\
    Copyright (C) 2020 Amélia O. F. da S.\
    Reachable at a.mellifluous.one@gmail.com\
    or www.github.com/m3101\
\
    This program is free software: you can redistribute it and/or modify\
    it under the terms of the GNU Affero General Public License as published\
    by the Free Software Foundation, either version 3 of the License, or\
    (at your option) any later version.\
\
    This program is distributed in the hope that it will be useful,\
    but WITHOUT ANY WARRANTY; without even the implied warranty of\
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\
    GNU Affero General Public License for more details.\
\
    You should have received a copy of the GNU Affero General Public License\
    along with this program.  If not, see <https://www.gnu.org/licenses/>.";

#include "PackedResources/resources.h"
#include "states.h"
#include "statelist.h"
#include "Audio/audiolist.h"
#include "../lib/Simple-SDL2-Audio/src/audio.h"
#include "sharedfonts.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<time.h>
#include <assert.h>

#if AC_USE_GL3W
#include <gl3w/GL/gl3w.h>

/*OpenGL VBO*/
static float screenVX[]={
    // positions   // texCoords
    -1.0f,  -1.0f,  0.0f, 1.0f,
    -1.0f, 1.0f,  0.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 0.0f,

    -1.0f, -1.0f,  0.0f, 1.0f,
    1.0f,  1.0f,  1.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 1.0f
};
/*OpenGL Shaders*/
extern char _binary_src_shaders_vhs_vert_start[];
extern char _binary_src_shaders_vhs_vert_end[];
extern char _binary_src_shaders_vhs_frag_start[];
extern char _binary_src_shaders_vhs_frag_end[];
#endif
#include <SDL2/SDL_opengl.h>

#if AC_USE_TTS
#include <espeak-ng/speak_lib.h>
#endif

#define WW 640
#define WH 480

TTF_Font* uni0553;

int main()
{
    /*Initializing everything*/
    gl3wInit();
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)return -1;
    TTF_Init();
    initAudio();

    srand(clock());

    /*Window creation (for OpenGL Context)*/
    SDL_Window* mwindow = SDL_CreateWindow("Acarajé V0.0.0-prealpha",0,0,WW,WH,SDL_WINDOW_OPENGL);
    if(mwindow == NULL)
    {
        SDL_Quit();
        return -1;
    }

    SDL_GLContext Context = SDL_GL_CreateContext(mwindow);

    SDL_Renderer* renderer = SDL_CreateRenderer(mwindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        SDL_DestroyWindow(mwindow);
        SDL_Quit();
        return -1;
    }

    #if AC_USE_TTS
    /*Initialise voice synth*/
    espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK, 500, NULL, 0);
    #endif

    /*Loading packed fonts*/
    uni0553=TTF_OpenFontRW(SDL_RWFromConstMem(_binary_uni0553_ttf_start,_binary_uni0553_ttf_end-_binary_uni0553_ttf_start),1,25);
    
    #if AC_USE_GL3W
    /*Loading packed shaders*/
    int success;
    char compileLog[512];

    int VHSVshader=glCreateShader(GL_VERTEX_SHADER);
    int VHSVlen=_binary_src_shaders_vhs_vert_end-_binary_src_shaders_vhs_vert_start;
    const char* bufferpointer=_binary_src_shaders_vhs_vert_start;
    glShaderSource(VHSVshader,1,&bufferpointer,&VHSVlen);
    glCompileShader(VHSVshader);
    glGetShaderiv(VHSVshader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(VHSVshader,512,NULL,compileLog);
        printf("Shader Compilation error:\n%s",compileLog);
        exit(1);
    }

    int VHSFshader=glCreateShader(GL_FRAGMENT_SHADER);
    int VHSFlen=_binary_src_shaders_vhs_frag_end-_binary_src_shaders_vhs_frag_start;
    bufferpointer=_binary_src_shaders_vhs_frag_start;
    glShaderSource(VHSFshader,1,&bufferpointer,&VHSFlen);
    glCompileShader(VHSFshader);
    glGetShaderiv(VHSFshader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(VHSFshader,512,NULL,compileLog);
        printf("Shader Compilation error:\n%s",compileLog);
        exit(1);
    }

    int shaderprog=glCreateProgram();
    glAttachShader(shaderprog,VHSVshader);
    glAttachShader(shaderprog,VHSFshader);
    glLinkProgram(shaderprog);
    glGetProgramiv(shaderprog,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(shaderprog,512,NULL,compileLog);
        printf("Shader linking error:\n%s",compileLog);
        exit(1);
    }
    printf("Shader Program ID %d\n",shaderprog);
    glDeleteShader(VHSFshader);
    glDeleteShader(VHSVshader);
    #endif
    SDL_Texture* screen=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,WW,WH);
    assert(screen);
    #if AC_USE_GL3W
    SDL_GL_BindTexture(screen,NULL,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    SDL_GL_UnbindTexture(screen);

    /*Screen vertices for OpenGL shaders*/
    unsigned int scrVBO,scrVAO;
    glGenVertexArrays(1,&scrVAO);
    glGenBuffers(1,&scrVBO);
    glBindVertexArray(scrVAO);
    glBindBuffer(GL_ARRAY_BUFFER,scrVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(screenVX),&screenVX,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(sizeof(float)*2));
    #endif

    /*Game things*/
    ac_state *cur_state=&LoadState,*next_state=NULL;
    loadAudios();

    /*Main loop*/
    char ac_flags=03;
    /*
    Flags:
    01 - Continue main loop (halts when unset)
    02 - Use shaders
    */

    #if AC_USE_GL3W
    int sid,tid,fid;
    glGetIntegerv(GL_CURRENT_PROGRAM,&sid);
    glUseProgram(shaderprog);
    GLint scrtexid=glGetUniformLocation(shaderprog,"scrTex");
    printf("scrtexid=%d\n",scrtexid);
    GLint tickid=glGetUniformLocation(shaderprog,"ticks");
    printf("ticks=%d\n",tickid);
    glUseProgram(sid);
    #endif

    uint32_t tstart,tend,frame_time=1000/30;
    cur_state->init(&cur_state,&next_state,renderer,mwindow,&ac_flags);
    SDL_Event evt;
    while(ac_flags&01)
    {
        tstart=SDL_GetTicks();
        SDL_SetRenderTarget(renderer,screen);
        #if AC_USE_GL3W
        glClearColor(0.0,0.0,0.0,0.0);
        #endif
        SDL_SetRenderDrawColor(renderer, 0,0,0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        /*Managing state changes*/
        if(next_state!=NULL)
        {
            cur_state->destroy();
            cur_state=next_state;
            next_state=NULL;
            cur_state->init(&cur_state,&next_state,renderer,mwindow,&ac_flags);
        }

        /*Event handling*/
        while(SDL_PollEvent(&evt))
        {
            if(evt.type == SDL_QUIT){ac_flags&=~01;}
            if(evt.type == SDL_MOUSEBUTTONDOWN){
                if(ac_flags&02) ac_flags&=~02;
                else ac_flags|=02;
            }
            cur_state->event(&cur_state,&next_state,renderer,mwindow,&ac_flags,&evt);
        }

        cur_state->frame(&cur_state,&next_state,renderer,mwindow,&ac_flags);

        /*Rendering*/
        SDL_RenderPresent(renderer);
        #if AC_USE_GL3W
        if(ac_flags&02)
        {
            glGetIntegerv(GL_CURRENT_PROGRAM,&sid);
            glGetIntegerv(GL_TEXTURE_BINDING_2D,&tid);
            glGetIntegerv(GL_FRAMEBUFFER_BINDING,&fid);

            glBindFramebuffer(GL_FRAMEBUFFER,0);
            glDisable(GL_DEPTH_TEST);
            glViewport(0, 0, WW, WH);
            glClearColor(1.f, 0.f, 1.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderprog);
            glBindVertexArray(scrVAO);
            glActiveTexture(GL_TEXTURE0);
            SDL_GL_BindTexture(screen,NULL,NULL);
            glUniform1i(scrtexid,0);
            glUniform1ui(tickid,tstart);
            glDrawArrays(GL_TRIANGLES,0,6);

            SDL_GL_SwapWindow(mwindow);

            SDL_GL_UnbindTexture(screen);
            glBindTexture(GL_TEXTURE_2D,tid);
            glUseProgram(sid);
            glBindFramebuffer(GL_FRAMEBUFFER,fid);
        }
        else
        {
        #endif
            SDL_SetRenderTarget(renderer,NULL);
            SDL_RenderCopy(renderer,screen,NULL,NULL);
            SDL_RenderPresent(renderer);
        #if AC_USE_GL3W
        }
        #endif
        SDL_GL_SwapWindow(mwindow);
        
        /*Framerate stabilization*/
        tend=SDL_GetTicks();
        if(tend-tstart<frame_time)
            SDL_Delay(frame_time-(tend-tstart));
    }
    freeAudios();
    endAudio();
    #if AC_USE_GL3W
    glDeleteProgram(shaderprog);
    glDeleteBuffers(1,&scrVBO);
    glDeleteVertexArrays(1,&scrVAO);
    #endif
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(mwindow);
    TTF_CloseFont(uni0553);
    TTF_Quit();
    SDL_Quit();
}
/*
    Copyright 2014 Julius Ikkala

    This file is part of GLSLTest.

    GLSLTest is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GLSLTest is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GLSLTest.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "sdl.h"

unsigned init_sdl(
    int width,
    int height,
    int fullscreen,
    int vsync,
    int gl_major,
    int gl_minor,
    struct sdl_res *res
){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    res->win=SDL_CreateWindow(
        "GLSLTest", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        width, height,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|
        (fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0)
    );
    res->ctx=SDL_GL_CreateContext(res->win);
    SDL_GL_MakeCurrent(res->win, res->ctx);
    SDL_GL_SetSwapInterval(vsync);
    return 0;
}
void deinit_sdl(struct sdl_res res)
{
    SDL_GL_DeleteContext(res.ctx);
    SDL_DestroyWindow(res.win);
    SDL_Quit();
}

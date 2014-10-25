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
/** \file glsltest.c
 *  \brief Contains the logic of the program
 */
#include "shader.h"
#include "parse_input.h"
#include <SDL2/SDL.h>
#include <stdio.h>

void print_usage(const char *name)
{
    printf("Usage: %s SOURCES [OPTIONS]\n"
           "    SOURCES: any number of GLSL fragment shader source file paths\n"
           "    OPTIONS: -w<width in pixels>\n"
           "             -h<height in pixels>\n"
           "             -f, --fullscreen\n"
           "             -v, --vsync\n"
           "             --glversion<MAJOR>.<MINOR>\n", name);
}

int main(int argc, char **argv)
{
    SDL_Window *win=NULL;
    SDL_GLContext ctx;
    SDL_Event e;
    unsigned quit=0;
    struct parsed_input p;
    p.win_w=640;
    p.win_h=480;
    p.fullscreen=0;
    p.vsync=0;
    p.version_major=3;
    p.version_minor=0;
    if(parse_input(argv, argc, &p))
    {
        print_usage(argv[0]);
        return 1;
    }
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, p.version_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, p.version_minor);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, p.vsync);
    SDL_GL_SetSwapInterval(p.vsync);
    
    win=SDL_CreateWindow(
        "GLSLTest", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        p.win_w, p.win_h,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|
        (p.fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0)
    );
    ctx=SDL_GL_CreateContext(win);
    SDL_GL_MakeCurrent(win, ctx);
    /*Print the state of the window*/
    SDL_GetWindowSize(win, &p.win_w, &p.win_h);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &p.version_major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &p.version_minor);
    printf(
        "OpenGL: %d.%d\nViewport: %dx%d\n",
        p.version_major, p.version_minor, p.win_w, p.win_h
    );
    free_parsed_input(p);
    
    /*Start rendering*/
    glViewport(0,0,p.win_w,p.win_h);
    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
            case SDL_QUIT:
                quit=1;
                break;
            case SDL_WINDOWEVENT:
                if(e.window.event==SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                   glViewport(0,0,e.window.data1,e.window.data2); 
                }
                break;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym==SDLK_ESCAPE)
                {
                    quit=1;
                }
                break;
            }
        }
        SDL_GL_SwapWindow(win);
    }
    
    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

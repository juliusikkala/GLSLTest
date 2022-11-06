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
#include "gl.h"
#include "io.h"
#include "sdl.h"
#include <stdio.h>
#include <stdlib.h>
/*The amount of seconds to measure the FPS for before printing*/
#define FPS_MEASURE_PERIOD 5

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

unsigned init(int argc, char **argv, struct sdl_res *sdl, struct gl_res *gl)
{
    char **srcs;
    unsigned err=0;
    struct parsed_input p;
    p.win_w=640;
    p.win_h=480;
    p.fullscreen=0;
    p.vsync=0;
    p.version_major=4;
    p.version_minor=6;
    
    /*Parse input*/
    if(parse_input(argv, argc, &p))
    {
        print_usage(argv[0]);
        return 1;
    }
    /*Check if there are no shaders specified*/
    if(p.shader_paths==NULL)
    {
        fprintf(stderr, "Fatal error: no input files\n");
        return 1;
    }
    /*Read shader sources now so we can free the parsed_input early*/
    if(!(srcs=read_text_files((const char * const*)p.shader_paths, p.shader_paths_sz)))
    {
        return 1;
    }
    
    free_parsed_input(p);

    /*Init SDL*/
    if(init_sdl(
        p.win_w,
        p.win_h,
        p.fullscreen,
        p.vsync,
        p.version_major,
        p.version_minor,
        sdl
    )){
        free_2d((void**)srcs, p.shader_paths_sz);
        return 1;
    }
    
    /*Print the state of the window*/
    SDL_GetWindowSize(sdl->win, &p.win_w, &p.win_h);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &p.version_major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &p.version_minor);
    printf(
        "OpenGL: %d.%d\nViewport: %dx%d\n",
        p.version_major, p.version_minor, p.win_w, p.win_h
    );
    
    /*Init OpenGL*/
    err=init_gl((const char * const *)srcs, p.shader_paths_sz, gl);
    free_2d((void**)srcs, p.shader_paths_sz);
    if(err){
        deinit_sdl(*sdl);
        return 1;
    }
    
    glUseProgram(gl->shader_program);
    glViewport(0,0,p.win_w,p.win_h);

    glUniform1i(gl->uniform_mouse_pressed, 0);
    glUniform2f(gl->uniform_res, p.win_w, p.win_h);
    return 0;
}

void deinit(struct sdl_res sdl, struct gl_res gl)
{
    deinit_gl(gl);
    deinit_sdl(sdl);
}
int main(int argc, char **argv)
{
    struct sdl_res sdl;
    struct gl_res gl;
    SDL_Event e;
    unsigned quit=0;
    unsigned begin_ticks;
    unsigned frames=0;
    unsigned frame_index=0;
    unsigned measure_ticks;
    
    if(init(argc, argv, &sdl, &gl))
    {
        return 1;
    }
    begin_ticks=measure_ticks=SDL_GetTicks();

    /*Start the main loop*/
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
                    glUniform2f(gl.uniform_res, e.window.data1, e.window.data2);
                }
                break;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym==SDLK_ESCAPE)
                {
                    quit=1;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                glUniform1i(gl.uniform_mouse_pressed, 1);
                break;
            case SDL_MOUSEBUTTONUP:
                glUniform1i(gl.uniform_mouse_pressed, 0);
                break;
            case SDL_MOUSEMOTION:
                glUniform2f(gl.uniform_mouse, e.motion.x, e.motion.y);
                break;
            }
        }
        frames++;
        frame_index++;
        if(SDL_GetTicks()-measure_ticks>FPS_MEASURE_PERIOD*1000)
        {
            printf("%d frames in %.1f seconds =  %.3f FPS\n",
                frames,
                (float) FPS_MEASURE_PERIOD,
                frames/(float) FPS_MEASURE_PERIOD
            );
            frames=0;
            measure_ticks=SDL_GetTicks();
        }
        glUniform1f(gl.uniform_time, (SDL_GetTicks()-begin_ticks)/1000.0f);
        glUniform1i(gl.uniform_frame, frame_index);
        render(&gl);
        SDL_GL_SwapWindow(sdl.win);
    }
    deinit(sdl, gl);
    return 0;
}

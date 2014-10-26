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
    p.version_major=3;
    p.version_minor=3;
    
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
    
    if(init(argc, argv, &sdl, &gl))
    {
        return 1;
    }

    /*Start rendering*/
    glClearColor(0.5,0.5,1.0,0.0);
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
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(gl.quad_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        SDL_GL_SwapWindow(sdl.win);
    }
    deinit(sdl, gl);
    return 0;
}

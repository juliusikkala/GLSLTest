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

static const GLfloat quad_buffer[6*3]={
     1.0f, 1.0f, 0,
     1.0f,-1.0f, 0,
    -1.0f, 1.0f, 0,
    -1.0f, 1.0f, 0,
     1.0f,-1.0f, 0,
    -1.0f,-1.0f, 0
};

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
    struct sdl_res res;
    SDL_Event e;
    unsigned quit=0;
    char **srcs=NULL;
    GLuint shader=0;
    GLenum err;
    GLuint buffer;
    GLuint vao;
    struct parsed_input p;
    p.win_w=640;
    p.win_h=480;
    p.fullscreen=0;
    p.vsync=0;
    p.version_major=3;
    p.version_minor=3;
    
    if(parse_input(argv, argc, &p))
    {
        print_usage(argv[0]);
        return 1;
    }
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
        &res)
    ){
        return 1;
    }
    
    /*Init GLEW*/
    glewExperimental=1;
    err=glewInit();
    if (err!=GLEW_OK)
    {
        fprintf(stderr, "%s\n", glewGetErrorString(err));
        return 1;
    }
    /*Print the state of the window*/
    SDL_GetWindowSize(res.win, &p.win_w, &p.win_h);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &p.version_major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &p.version_minor);
    printf(
        "OpenGL: %d.%d\nViewport: %dx%d\n",
        p.version_major, p.version_minor, p.win_w, p.win_h
    );
    /*Generate VAO*/
    glGenVertexArrays(1, &vao);
    glBindVertexArray(0);
    
    /*Load buffers*/
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_buffer), quad_buffer, GL_STATIC_DRAW);
    
    /*Fill VAO*/
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);
    
    /*Load shaders*/
    shader=compile_shader_program(srcs, p.shader_paths_sz);
    free_2d((void**)srcs, p.shader_paths_sz);
    if(shader==0)
    {
        goto end;
    }
    glUseProgram(shader);
    /*Start rendering*/
    glViewport(0,0,p.win_w,p.win_h);
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
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        SDL_GL_SwapWindow(res.win);
    }
end:
    /*Clean up*/
        
    glDeleteProgram(shader);
    glDeleteBuffers(1, &buffer);
    glDeleteVertexArrays(1, &vao);
    deinit_sdl(res);
    return 0;
}

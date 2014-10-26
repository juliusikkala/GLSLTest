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
#include "gl.h"
#include <stdio.h>
#include <stdlib.h>

static const char * const vshader_src=
"#version 330 core\n"
"layout(location=0) in vec2 vertex;\n"
"out vec2 uv;\n"
"void main(void)\n"
"{\n"
"    gl_Position=vec4(vertex, 0, 1.0);\n"
"    uv=vertex;\n"
"}\n"; 

GLuint compile_shader_program(char **srcs, unsigned srcs_sz)
{
    #define check_shader(var) \
        do{\
        glGetShaderiv(var, GL_COMPILE_STATUS, &status);\
        if(status==GL_FALSE)\
        {/*Compilation failed.*/\
            glGetShaderiv(var, GL_INFO_LOG_LENGTH, &log_length);\
            log=malloc(log_length*sizeof(char));\
            glGetShaderInfoLog(var, log_length, NULL, (GLchar *)log);\
            fprintf(stderr, "%s\n", log);\
            free(log);\
            goto failed;\
        }}while(0)
    
    GLint status=GL_FALSE;
    GLint log_length=0;
    char *log=NULL;
    GLuint vshader=glCreateShader(GL_VERTEX_SHADER);
    GLuint fshader=glCreateShader(GL_FRAGMENT_SHADER);
    GLuint program;
    
    glShaderSource(vshader, 1, &vshader_src, NULL);
    glShaderSource(fshader, srcs_sz, (const GLchar * const*)srcs, NULL);
    
    glCompileShader(vshader);
    check_shader(vshader);

    glCompileShader(fshader);
    check_shader(fshader);
    
    program=glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status==GL_FALSE)
    {/*Linking failed.*/
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        glGetProgramInfoLog(program, log_length, NULL, (GLchar *)log);
        fprintf(stderr, "%s\n", log);
        free(log);
        glDeleteProgram(program);
        goto failed;
    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return program;
failed:
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return 0;
    #undef check_shader
}

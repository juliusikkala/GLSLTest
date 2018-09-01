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


unsigned init_glew()
{
    GLenum err;
    glewExperimental=1;
    err=glewInit();
    if (err!=GLEW_OK)
    {
        fprintf(stderr, "%s\n", glewGetErrorString(err));
        return 1;
    }
    return 0;
}

GLuint compile_shader(GLenum shader_type, const char * const *srcs, unsigned srcs_sz)
{
    GLint status=GL_FALSE;
    char *log=NULL;
    GLint log_length=0;
    GLuint shader=glCreateShader(shader_type);
    
    glShaderSource(shader, srcs_sz, (const GLchar **)srcs, NULL);
    
    glCompileShader(shader);
    
    /*Check whether the compilation was successful*/
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status==GL_FALSE)
    {/*Compilation failed.*/
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        log=malloc(log_length*sizeof(char));
        glGetShaderInfoLog(shader, log_length, NULL, (GLchar *)log);
        fprintf(stderr, "%s\n", log);
        free(log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint link_shader_program(GLuint *shaders, unsigned shaders_sz)
{
    GLint status=GL_FALSE;
    char *log=NULL;
    GLint log_length=0;
    GLuint program=glCreateProgram();
    
    for(unsigned i=0;i<shaders_sz;++i)
    {
        glAttachShader(program, shaders[i]);
    }
    glLinkProgram(program);

    /*Check whether the linking was successful*/
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status==GL_FALSE)
    {/*Linking failed.*/
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        glGetProgramInfoLog(program, log_length, NULL, (GLchar *)log);
        fprintf(stderr, "%s\n", log);
        free(log);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

static const char * const vshader_src=
"#version 430 core\n"
"layout(location=0) in vec2 vertex;\n"
"out vec2 uv;\n"
"void main(void)\n"
"{\n"
"    gl_Position=vec4(vertex, 0, 1.0);\n"
"    uv=vertex;\n"
"}\n"; 

GLuint compile_shader_program(const char * const *srcs, unsigned srcs_sz)
{
    GLuint shaders[2]={0,0};
    GLuint program=0;
    
    if(!(shaders[0]=compile_shader(GL_VERTEX_SHADER, &vshader_src, 1)))
    {/*Failed to compile the vertex shader*/
        return 0;
    }
    if(!(shaders[1]=compile_shader(GL_FRAGMENT_SHADER, srcs, srcs_sz)))
    {/*Failed to compile the fragment shader*/
        glDeleteShader(shaders[0]);/*Delete the vertex shader*/
        return 0;
    }
    if(!(program=link_shader_program(shaders, 2)))
    {/*Failed to link the program*/
        /*Delete the shaders, they are no longer needed.*/
        glDeleteShader(shaders[0]);
        glDeleteShader(shaders[1]); 
        return 0;   
    }
    return program;
}

GLuint create_vbo(const void *data, size_t data_bytes, GLenum usage)
{
    GLuint vbo=0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data_bytes, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

GLuint create_vao(
    GLuint *attrib_indices,
    GLuint *vbos,
    GLint *sizes,
    GLenum *types,
    unsigned amount
)
{
    GLuint vao=0;
    /*Generate VAO*/
    glGenVertexArrays(1, &vao);
    
    /*Fill VAO*/
    glBindVertexArray(vao);

    for(unsigned i=0;i<amount;++i)
    {
        glEnableVertexAttribArray(attrib_indices[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glVertexAttribPointer(
            attrib_indices[i],
            sizes[i],
            types[i],
            GL_FALSE,
            0,
            0
        );
    }
    
    /*Unbind the VAO*/
    glBindVertexArray(0);
    
    /*Now disable the enabled vertex attrib arrays*/
    for(unsigned i=0;i<amount;++i)
    {
        glDisableVertexAttribArray(attrib_indices[i]);
    }
    return vao;
}

static const GLfloat quad_buffer[6*3]={
     1.0f, 1.0f, 0,
     1.0f,-1.0f, 0,
    -1.0f, 1.0f, 0,
    -1.0f, 1.0f, 0,
     1.0f,-1.0f, 0,
    -1.0f,-1.0f, 0
};

unsigned init_gl(
    const char * const *shader_srcs,
    unsigned shader_srcs_sz,
    struct gl_res *res
)
{
    GLuint index=0;
    GLint size=3;
    GLenum type=GL_FLOAT;
    char *tex_data=NULL;
    unsigned i=0;
    if(init_glew())
        return 1;
    
    res->quad_vbo=create_vbo(quad_buffer, sizeof(quad_buffer), GL_STATIC_DRAW);
    res->quad_vao=create_vao(&index,&res->quad_vbo,&size,&type, 1);
    res->shader_program=compile_shader_program(shader_srcs, shader_srcs_sz);
    if(res->shader_program==0)
    {/*Shader program compilation failed*/
        glDeleteBuffers(1, &res->quad_vbo);
        glDeleteVertexArrays(1, &res->quad_vao);
        return 1;
    }
    
    /*Get the uniforms*/
    res->uniform_time=glGetUniformLocation(res->shader_program, "time");
    res->uniform_mouse=glGetUniformLocation(res->shader_program, "mouse");
    res->uniform_mouse_pressed=glGetUniformLocation(res->shader_program, "mouse_pressed");
    res->uniform_res=glGetUniformLocation(res->shader_program, "res");
    res->uniform_random_tex=glGetUniformLocation(
        res->shader_program,
        "random_tex"
    );
    /*Generate the random texture*/
    glGenTextures(1, &res->random_tex);
    glUniform1i(res->uniform_random_tex, 0);
    glActiveTexture(GL_TEXTURE0+0);
    glBindTexture(GL_TEXTURE_2D, res->random_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex_data=malloc(64*64*3);
    for(i=0;i<64*64*3;++i)
    {
        tex_data[i]=rand()%256;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
    free(tex_data);
    return 0;
}

void deinit_gl(struct gl_res res)
{
    glDeleteTextures(1, &res.random_tex);
    glDeleteBuffers(1, &res.quad_vbo);
    glDeleteVertexArrays(1, &res.quad_vao);
    glDeleteProgram(res.shader_program);
}

void render(struct gl_res *res)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(res->quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

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
/*! \file gl.h
 *  \brief Contains everything related to OpenGL.
 */
#ifndef GLSLTEST_GL_H_
#define GLSLTEST_GL_H_
    #include "GL/glew.h"
    #include "GL/gl.h"

    /*! \brief Contains the OpenGL resources used by GLSLTest
     */
    struct gl_res
    {
        /*! \brief The shader this program runs
         */
        GLuint shader_program;
        /*! \brief The VAO of the quad that is used to fill the screen
         */
        GLuint quad_vbo;
        /*! \brief The VBO of the quad that is used to fill the screen
         */
        GLuint quad_vao;
        /*! \brief The uniform location of \p time in the shader. A float in seconds.
         */
        GLint uniform_time;
        /*! \brief The uniform location of \p mouse in the shader. A vec2 containing
         *  the screen coordinates of the mouse.
         */
        GLint uniform_mouse;
        /*! \brief The uniform location of \p mouse_pressed in the shader. An int
         *  which is 1 when the mouse is pressed and 0 when it is not.
         */
        GLint uniform_mouse_pressed;
        /*! \brief The uniform location of \p res in the shader. An vec2 containing
         *  the screen resolution.
         */
        GLint uniform_res;
    };

    /*! \brief Initializes GLEW.
     *  \return non-zero on failure.
     *  \note Prints the errors in case of failure
     */
    unsigned init_glew();

    /*! \brief Compiles a shader
     *  \param[in] shader_type The type of the OpenGL shader
     *  \param[in] srcs The list of sources for the shader. Each must be NULL-terminated.
     *  \param[in] srcs_sz The amount of sources in \p srcs.
     *  \return The compiled shader. 0 on error.
     */
    GLuint compile_shader(GLenum shader_type, const char * const *srcs, unsigned srcs_sz);

    /*! \brief Links a shader program
     *  \param[in] shaders The list of shaders for the program
     *  \param[in] shaders_sz The amount of shaders in \p shaders.
     *  \return The linked shader program. 0 on error.
     */
    GLuint link_shader_program(GLuint *shaders, unsigned shaders_sz);

    /*! \brief Compiles and links the shaders given as arguments 
     *  \param[in] srcs The list of sources. Each must be NULL-terminated.
     *  \param[in] srcs_sz The amount of sources in \p srcs.
     *  \return The compiled and linked shader program. 0 on error.
     */
    GLuint compile_shader_program(const char * const *srcs, unsigned srcs_sz);
    
    /*! \brief Creates an OpenGL Vertex Buffer Object
     *  \param[in] data The data the VBO should contain
     *  \param[in] data_bytes Amount of bytes of data \p data contains
     *  \param[in] usage How the VBO is going to be used.
     *  \return The created VBO
     */
    GLuint create_vbo(const void *data, size_t data_bytes, GLenum usage);

    /*! \brief Creates an OpenGL Vertex Array Object
     *  \param[in] attrib_indices Indices of the attributes
     *  \param[in] vbos The VBOs bound to the indices
     *  \param[in] sizes The sizes of the VBO elements. 1, 2, 3 or 4
     *  \param[in] types The types of the VBO elements.
     *  \param[in] amount The amount of elements all of the following arrays contain
     *  \return The created VAO
     */
    GLuint create_vao(
        GLuint *attrib_indices,
        GLuint *vbos,
        GLint *sizes,
        GLenum *types,
        unsigned amount
    );
    
    /*! \brief Initializes all the OpenGL resources used by GLSLTest
     *  \param[in] shader_srcs The source codes of the fragment shader
     *  \param[in] shader_srcs_sz The amount of shader source codes
     *  \param[out] res The OpenGL resources
     *  \return non-zero on failure
     */
    unsigned init_gl(
        const char * const *shader_srcs,
        unsigned shader_srcs_sz,
        struct gl_res *res
    );
    
    /*! \brief Deinitializes all the OpenGL resources used by GLSLTest
     *  \param[in] res The OpenGL resources
     */
    void deinit_gl(struct gl_res res);
    
    /*! \brief Does the rendering of the shader
     *  \param[in] res The OpenGL resources
     */
    void render(struct gl_res *res);
#endif

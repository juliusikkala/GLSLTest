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
/*! \file shader.h
 *  \brief Contains everything related to loading the shader given to GLSLTest
 */
#ifndef GLSLTEST_SHADER_H_
#define GLSLTEST_SHADER_H_
    #include "GL/glew.h"
    #include "GL/gl.h"

    /*! \brief Compiles and links the shaders given as arguments 
     *  \param[in] srcs The list of sources. Each must be NULL-terminated.
     *  \param[in] srcs_sz The amount of sources in \p srcs.
     *  \return The compiled and linked shader. 0 if invalid, call get_shader_error()
     *  \see get_shader_error()
     */
    GLuint load_shader(char **srcs, unsigned srcs_sz);
    
    /*! \brief Returns the error caused by a failed call to load_shader()
     *  \return The error message. NULL if there was no error.
     *  \warning Remember to free() the returned char*!
     */
    char *get_shader_error();

#endif

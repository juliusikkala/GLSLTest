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
/*! \file sdl.h
 *  \brief Contains everything related to SDL.
 */
#ifndef GLSLTEST_SDL_H_
#define GLSLTEST_SDL_H_
    #include "SDL2/SDL.h"
    
    /*! \brief Contains the SDL resources used by GLSLTest
     */
    struct sdl_res
    {
        SDL_Window *win;
        SDL_GLContext ctx;
    };

    /*! \brief Initializes SDL, opens the window and prints possible error codes
     *  \param[in] width Width of the window
     *  \param[in] height Height of the window
     *  \param[in] fullscreen Fullscreen switch. 0 if off, 1 if on
     *  \param[in] vsync Vertical synchronization switch. 0 if off, 1 if on
     *  \param[in] gl_major Major number of the preferred OpenGL version
     *  \param[in] gl_minor Minor number of the preferred OpenGL version
     *  \param[out] res Resources allocated by this function
     *  \return non-zero on failure. On failure, do not deinit_sdl, it has been
     *  done automatically.
     */
    unsigned init_sdl(
        int width,
        int height,
        int fullscreen,
        int vsync,
        int gl_major,
        int gl_minor,
        struct sdl_res *res
    );
    /*! \brief Deinitializes SDL.
     *  \note Only use if init_sdl was successful!
     */
    void deinit_sdl(struct sdl_res res);
#endif

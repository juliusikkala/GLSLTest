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
/*! \file parse_input.h
 *  \brief Contains everything related to parsing the input given to GLSLTest
 */
#ifndef GLSLTEST_PARSE_INPUT_H_
#define GLSLTEST_PARSE_INPUT_H_

    /*! \brief Contains all variables parsed from input
     *  \see parse_input()
     *  \see free_parsed_input();
     */
    struct parsed_input
    {
        /*! \brief List of paths to GLSL shader sources
         *  \note The first pointer will be NULL if unspecified paths
         *  \note Not the source code!
         */
        const char **shader_paths;
        /*! \brief Amount of shader sources in \ref shader_paths
         */
        unsigned shader_paths_sz;
        /*! \brief The width of the window
         *  \note <0 if unspecified.
         */
        int win_w;
        /*! \brief The height of the window
         *  \note <0 if unspecified.
         */
        int win_h;
        /*! \brief The switch for selecting whether a window should cover the display
         *  \note >1 if on, 0 if off, <0 if unspecified
         */
        int fullscreen;
        /*! \brief The OpenGL major version number
         *  \note <0 if unspecified
         */
        int version_major;
        /*! \brief The OpenGL minor version number
         *  \note <0 if unspecified
         */
        int version_minor;
        /*! \brief Vertical synchronization switch
         *  \note >0 if on, 0 if off, <0 if unspecified
         */
        int vsync;
    };

    /*! \brief Parses command-line arguments into a readable format
     *  \param[in] argv The list of arguments given to main().
     *  \param[in] argc The amount of members \p argv contains
     *  \param[out] p The parsed arguments
     *  \return non-zero in case of failure.
     *  \see parsed_input
     */
    unsigned parse_input(const char **argv, int argc, struct parsed_input *p);
    
    /*! \brief Helper function which frees the memory used by pointers in \p p
     *  \param[in] p The struct you wish to free
     *  \see parsed_input
     */
    void free_parsed_input(struct parsed_input p);
#endif

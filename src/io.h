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
/*! \file io.h
 *  \brief Contains everything related to input and output to GLSLTest
 */
#ifndef GLSLTEST_IO_H_
#define GLSLTEST_IO_H_
    #include <stddef.h>
    /*! \brief Contains all variables parsed from input
     *  \see parse_input()
     *  \see free_parsed_input();
     */
    struct parsed_input
    {
        /*! \brief List of paths to GLSL shader sources
         *  \note Not the source code!
         */
        char **shader_paths;
        /*! \brief Amount of shader sources in \ref shader_paths
         */
        unsigned shader_paths_sz;
        /*! \brief The width of the window
         */
        int win_w;
        /*! \brief The height of the window
         */
        int win_h;
        /*! \brief The switch for selecting whether a window should cover the display
         *  \note Set to 1 for on, 0 for off
         */
        int fullscreen;
        /*! \brief Vertical synchronization switch
         *  \note Set to 1 for on, 0 for off
         */
        int vsync;
        /*! \brief The OpenGL major version number
         */
        int version_major;
        /*! \brief The OpenGL minor version number
         */
        int version_minor;
        /*! \brief Paths to the textures to use
         */
        char **texture_paths;
        /*! \brief Amount of texture paths in \p texture_paths
         */
        size_t texture_paths_sz;
    };

    /*! \brief Parses command-line arguments into a readable format
     *  \param[in] argv The list of arguments given to main().
     *  \param[in] argc The amount of members \p argv contains
     *  \param[out] p The parsed arguments \note Sets only the parameters given
     *  in arguments, others will not be touched, except for parsed_input#shader_paths and 
     *  parsed_input#shader_paths_sz
     *  \return non-zero in case of failure.
     *  \see parsed_input
     */
    unsigned parse_input(char **argv, int argc, struct parsed_input *p);
    
    /*! \brief Helper function which frees the memory used by pointers in \p p
     *  \param[in] p The struct you wish to free
     *  \see parsed_input
     */
    void free_parsed_input(struct parsed_input p);
    
    /*! \brief Reads files referenced by \p paths
     *  \param[in] paths Array of filesystem paths which point to the files to be read
     *  \param[in] paths_sz Amount of paths in \p paths
     *  \return Array of NULL-terminated string containing the files' contents.
     *          NULL if \p paths == NULL, \p paths_sz == 0 or any one of \p paths is
     *          invalid.
     *  \note Writes error output to stderr for every invalid path
     */
    char **read_text_files(const char * const *paths, unsigned paths_sz);

    /*! \brief An image struct.*/
    struct image {
        /*! \brief Width of the image */
        int w;
        /*! \brief Height of the image */
        int h;
        /*! \brief Image data, in RGB8 */
        char *data;
    };
    /*! \brief Reads images referenced by \p paths
     *  \param[in] paths Array of filesystem paths which point to the images to be read
     *  \param[in] paths_sz Amount of paths in \p paths
     *  \return Array of NULL-terminated string containing the images' contents.
     *          NULL if \p paths == NULL, \p paths_sz == 0 or any one of \p paths is
     *          invalid.
     *  \note Writes error output to stderr for every invalid path
     */
    struct image *read_image_files(const char * const *paths, unsigned paths_sz);
    
    /*! \brief Frees the two-dimensional argument \p p
     *  \param[in] p Two-dimensional array
     *  \param[in] p_sz amount of members in \p p
     */
    void free_2d(void **p, unsigned p_sz);
#endif

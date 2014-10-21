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
#include "shader.h"
#include "parse_input.h"
#include <stdio.h>

void print_usage(const char *name)
{
    printf("Usage: %s SOURCES [OPTIONS]\n"
           "    SOURCES: any number of GLSL fragment shader source file paths\n"
           "    OPTIONS: -w<width in pixels>\n"
           "             -h<height in pixels>\n"
           "             -f, --fullscreen\n"
           "             -s, --vsync\n"
           "             --glversion<MAJOR>.<MINOR>\n", name);
}

int main(int argc, char **argv)
{
    struct parsed_input p;
    if(parse_input(argv, argc, &p))
    {
        print_usage(argv[0]);
        return 1;
    }
    free_parsed_input(p);
    return 0;
}

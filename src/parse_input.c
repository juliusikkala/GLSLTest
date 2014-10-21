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
#include "parse_input.h"
#include <stdlib.h>
#include <string.h>

unsigned parse_input(char **argv, int argc, struct parsed_input *p)
{
    return 1;
}
void free_parsed_input(struct parsed_input p)
{
    unsigned i;
    for(i=0;i<p.shader_paths_sz;i++)
    {
        free(p.shader_paths[i]);
    }
    free(p.shader_paths);
}

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
    int i=1;
    char *endptr=NULL;
    int temp1=0, temp2=0;
    p->shader_paths=NULL;
    p->shader_paths_sz=0;
    for(;i<argc;++i)
    {
        if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--fullscreen"))
        {/*The fullscreen switch*/
            p->fullscreen=1;
        }
        else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--vsync"))
        {/*The vsync switch*/
            p->vsync=1;
        }
        else if(!strncmp(argv[i], "-w", 2))
        {/*The width*/
            temp1=strtol(argv[i]+2, &endptr, 10);
            if(endptr!=argv[i]+strlen(argv[i]) || strlen(argv[i])<3)
            {
                return 1;
            }
            p->win_w=temp1;
        }
        else if(!strncmp(argv[i], "-h", 2))
        {/*The height*/
            temp1=strtol(argv[i]+2, &endptr, 10);
            if(endptr!=argv[i]+strlen(argv[i]) || strlen(argv[i])<3)
            {
                return 1;
            }
            p->win_h=temp1;
        }
        else if(!strncmp(argv[i], "--glversion", 11))
        {/*The OpenGL version*/
            temp1=strtol(argv[i]+11, &endptr, 10);
            if(endptr[0]!='.')
            {
                return 1;
            }
            temp2=strtol(endptr+1, &endptr, 10);
            if(endptr!=argv[i]+strlen(argv[i]))
            {
                return 1;
            }
            p->version_major=temp1;
            p->version_minor=temp2;
        }
        else if(argv[i][0]=='-')
        {/*Not a real switch*/
            return 1;
        }
        else
        {/*Probably a path*/
            p->shader_paths=realloc(p->shader_paths, (++p->shader_paths_sz)*sizeof(char*));
            p->shader_paths[p->shader_paths_sz-1]=
                malloc((strlen(argv[i])+1)*sizeof(char));
            memcpy(p->shader_paths[p->shader_paths_sz-1], argv[i], strlen(argv[i])+1);
        }
    }
    return 0;
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

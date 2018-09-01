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
#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned parse_input(char **argv, int argc, struct parsed_input *p)
{
    char *endptr=NULL;
    int temp1=0, temp2=0;
    p->shader_paths=NULL;/*Set these to empty by default*/
    p->shader_paths_sz=0;
    p->texture_paths=NULL;
    p->texture_paths_sz=0;

    for(int i=1;i<argc;++i)
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
        else if(!strncmp(argv[i], "-t", 2) && strlen(argv[i])>2)
        {/*Texture*/
            p->texture_paths=realloc(p->texture_paths, (++p->texture_paths_sz)*sizeof(char*));
            p->texture_paths[p->texture_paths_sz-1]=
                malloc((strlen(argv[i]+2)+1)*sizeof(char));
            memcpy(p->texture_paths[p->texture_paths_sz-1], argv[i]+2, strlen(argv[i]+2)+1);
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
    free_2d((void**)p.shader_paths, p.shader_paths_sz);
}

char **read_text_files(const char * const * paths, unsigned paths_sz)
{
    /*Freed and set to NULL mid-loop if a path is invalid*/
    char **file_datas=malloc(sizeof(char *)*paths_sz);
    FILE *file=NULL;
    long int file_size=0;

    if(paths==NULL||paths_sz==0)
    {/*No paths specified; return NULL.*/
        free(file_datas);
        return NULL;
    }
    for(int i=0;i<paths_sz;++i)
    {
        file=fopen(paths[i], "rb");
        if(file==NULL)
        {/*Could not open file*/
            fprintf(stderr, "Cannot read %s: No such file or directory\n", paths[i]);
            if(file_datas!=NULL)
            {/*No point reading the rest, but keep checking them all so we can
               warn about every invalid path*/
                free(file_datas);
                file_datas=NULL;
            }
            continue;
        }
        if(file_datas==NULL)
        {/*Reading all the files has already failed even though this file was
           readable. Close and continue checking.*/
            fclose(file);
            continue;
        }
        /*Get the size of the file so that it can be read in one go.*/
        fseek(file, 0, SEEK_END);
        file_size=ftell(file);
        fseek(file, 0, SEEK_SET);
        /*Make room for the file contents*/
        file_datas[i]=malloc((file_size+1)*sizeof(char));
        file_datas[i][file_size]=0;/*Null terminator*/
        fread((void *)file_datas[i], 1, file_size, file);
        fclose(file);
    }
    return file_datas;/*Note: may be NULL! (from the loop)*/
}

void free_2d(void **p, unsigned p_sz)
{
    for(unsigned i=0;i<p_sz;++i)
    {
        free(p[i]);
    }
    free(p);
}

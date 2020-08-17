/*
 * client for userland execve - (C) Innosecc.com 2009
 * <ryan@innosecc.com>
 *  
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/stat.h>

int
main(int argc, char **argv)
{
        char *file, *phrase;
        char **args;
        struct stat st;

        void (*ul_exec)(char *, char*, int, char**, int);
        void *buf, *handle;

        if (argc < 2)
        {
                printf("Usage: %s --phrase <pass phrase> <target> <args>\n", argv[0]);
                printf("Example ->\n"
                       "%s --phrase hello testprog\n"
                       "%s testprog arg1\n"
                       "%s testprog\n", argv[0],argv[0],argv[0]);
                exit(0);
        }
        if (!strcmp(argv[1], "--phrase"))
        {
                file = argv[3];
                phrase = argv[2];
                argc -= 3;
                argv += 3;
        }
        else
        {
                phrase = NULL;
                file = argv[1];
                argv += 1;
                argc -= 1;
        }

        if (stat(file, &st) < 0)
        {
                perror("stat");
                exit(-1);
        }

        userland_exec(file, phrase, argc, argv, st.st_size);
        exit(0);
}


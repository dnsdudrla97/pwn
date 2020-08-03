// gcc -o fp_vtable fp_vtable.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
char name[256] = "\0";
FILE *fp = NULL;
void getshell() {
        system("/bin/sh");
}       
int main()
{
        int bytes;
        char random[4];
        fp = fopen("/dev/urandom", "r");
        printf("Name: ");
        fflush(stdout);
        gets(name);
        
        fread(random, 1, 4, fp);
        
        printf("random: %s", random);
        return 0;
}
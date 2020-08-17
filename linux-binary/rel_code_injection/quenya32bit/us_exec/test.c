#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc > 1)
		while (argc)
		{
			printf("argv[] %s\n", argv[argc-1]);
			argc--;
		}
				
	
}


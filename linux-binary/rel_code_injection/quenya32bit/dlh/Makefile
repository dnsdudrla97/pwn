all:
	 gcc dlh.c -o dlh
	 gcc -fPIC -c example_lib.c -nostdlib
	 ld -shared -o example_lib.so.1.0 example_lib.o
clean:
	rm -f *.o *.so dlh

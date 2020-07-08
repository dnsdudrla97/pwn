#include <stdio.h>
#include <stdlib.h>
int main(void){
  char * buf = (char *)calloc(1, 4096);
  FILE * fp = 0;
  size_t sz = 0;
  fp = fopen("/proc/self/maps", "r");
  sz = fread(buf, 1, 4096, fp);
  fwrite(buf, 1, sz, stdout);
}

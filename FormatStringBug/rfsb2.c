#include <stdio.h>
int main(void) {
    int auth = "SECRET";
    char buf[32] = {0, };
    
    read(0, buf, 31);
    printf(buf);
}
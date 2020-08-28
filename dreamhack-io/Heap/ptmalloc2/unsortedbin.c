#include <stdlib.h>

int main(void) {
    int *a = malloc(200);
    int *b = malloc(200);
    int *c = malloc(200);

    free(a);
    int *d = malloc(129);

    free(b);
    free(c);

    return 0;
}
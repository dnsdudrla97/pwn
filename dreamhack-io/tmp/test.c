#include <stdio.h>

void trans(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int main() {
    int a = 10;
    int b = 20;
    printf("a: %d b: %d\n", a, b);
    trans(&a, &b);
    printf("a: %d b: %d\n", a, b);

    return 0;
}

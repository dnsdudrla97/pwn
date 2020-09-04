#include <iostream>

struct S {
    void f();
};

int main() {
    S *s = new S;
    delete s;
    s->f();
}
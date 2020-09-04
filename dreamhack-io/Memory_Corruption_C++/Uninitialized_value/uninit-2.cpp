#include <iostream>
class S {
    int c;
    public:
        int f(int i) const { return i + c;}
};

int f() {
    S s;
    return s.f(10);
}

int main() {
    int val = f();
    std::cout << val << std::endl;
}
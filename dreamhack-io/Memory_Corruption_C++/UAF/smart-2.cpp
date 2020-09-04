#include <iostream>
#include <memory>

struct B {
    virtual ~B() = default;
    //...
};
struct D : B {};

void g(std::shared_ptr<D> derived) {
    std::cout << "hi im g!" << std::endl;
};

int main() {
    std::shared_ptr<B> poly(new D);
    //..
    g(std::shared_ptr<D>(dynamic_cast<D *>(poly.get())));
}
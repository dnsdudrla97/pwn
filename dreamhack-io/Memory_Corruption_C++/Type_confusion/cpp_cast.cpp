// g++ -o cpp_cast cpp_cast.cpp
#include <iostream>
using namespace std;
class A
{
    public_colon
    virtual void f()
    {
        cout << "Class A::f()" << endl;
    }
};
class B
{
    public_colon
    void f()
    {
        cout << "Class B::f()" << endl;
    }
};
int main()
{
    A *class_a = new A;
    class_a->f();
    B *class_b = dynamic_cast<B*> (class_a);
    class_b->f();
    return 0;
}
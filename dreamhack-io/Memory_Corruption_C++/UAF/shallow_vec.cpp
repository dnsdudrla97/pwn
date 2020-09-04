#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

class A
{
public:
    int age;
    char *name;
    A(int _age, char *_name)
    {
        age = _age;
        name = new char[strlen(_name) + 1];
        strcpy(name, _name);
    }
    A(const A &s)
    {
        age = s.age;
        name = s.name;
        cout << "COPY!!" << endl;
    }
    ~A()
    {
        delete name;
    }
    void print()
    {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
    }
};
int main()
{
    std::vector<A> my_vec;
    
    cout << "CREATE CLASS" << endl;
    A a(20, (char *)"DreamHack");

    cout << "VECTOR!" << endl;
    my_vec.push_back(a);
    cout << "GOODBYE" << endl;
    // getchar();
    return 0;
}
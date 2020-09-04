
#include <iostream>
#include <string.h>

using namespace std;
class A
{
public:
    int age;
    char *name;
    A(int _age, char *_name) {
        age = _age;
        name = new char [strlen(_name)+1];
        strcpy(name,_name);
    }
    A(const A& s) 
    {
        age = s.age;
        name = s.name;
    }
    ~A() {
        // delete name; 
    }
    void print()
    {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
    }
};
int main()
{
    A a(20, (char *)"DreamHack");
    A b = a;
    strcpy(a.name, "C++ HACK!");
    a.print();
    b.print();
}
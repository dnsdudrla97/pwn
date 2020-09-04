// g++ -o wrong_cast wrong_cast.cpp
#include <iostream>
#include <string.h>
using namespace std;

class Parent
{
    public:
};

class Print : public Parent
{
    public:
    virtual void print_str(char *str)
    {
        cout << str << endl;
    }

    ~Print()
    {
    }
};

class Read : public Parent
{
    public:
    virtual void read_str(char *str)
    {
        cout << "Input: " << str << endl;
        cin >> str;
        cout << "Data: " << str << endl;
    }
    ~Read()
    {
    }
};

int main()
{
    Parent *p1 = new Print();
    Parent *p2 = new Read();
    Print *b1;

    char buf[256];
    strcpy(buf, "I'm print_str");

    b1 = static_cast<Print *>(p1);
    b1->print_str(buf);
    b1 = static_cast<Print *>(p2);
    b1->print_str(buf);

    return 0;
}
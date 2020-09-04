#include <iostream>
#include <string>

std::string str_func() {
    std::string a = "aaaa";
    return a;
}

void display_string(const char *buf) {
    std::cout << buf << std::endl;
}

int main() {
    const char *str = str_func().c_str();
    display_string(str);
    std::string b = "bbbb";
    display_string(str);
}
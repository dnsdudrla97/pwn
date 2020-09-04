#include <iostream>

class S {
    public:
        S() : mem(0), mem_size(0) { }

        S(int mem_size) {
            this->mem_size = mem_size;
            if (mem_size > 0) {
                this->mem = new char[mem_size];
            }
        }
        char *mem;
        int mem_size;
};

int main(){
    S s(-1);
    std::cout << s.mem << std::endl;
}
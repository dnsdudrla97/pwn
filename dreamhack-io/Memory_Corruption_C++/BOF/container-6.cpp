// g++ -o container-6 container-6.cpp -std=c++11
#include <iostream>
#include <vector>
void f(const std::vector<int> &c) {
	for(auto i = c.begin(), e = i + c.size(); i != e; ++i) {
		std::cout << *i << std::endl;
	}
}
int main(void) {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    f(v);
}

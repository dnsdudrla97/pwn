
#include <algorithm>
#include <vector>
#include <iostream>
void f(const std::vector<int> &src) {
        std::vector<int> dest(src);
        std::copy(src.begin(), src.end(), dest.begin());
}
int main(void){
        int size = 0;;
        std::vector<int> v;
        std::cin >> size;
        v.resize(size);
        v.assign(size, 0x41414141);
        f(v);
}
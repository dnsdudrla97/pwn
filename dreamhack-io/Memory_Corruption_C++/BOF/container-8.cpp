// g++ -o container-8 container-8.cpp
#include <vector>
#include <iostream>
void insert_table(std::vector<int> &table, unsigned long long pos, int value) {
    if(pos >= (long long)table.size()){
        std::cout << "overflow!" << std::endl;
        return;
    }
    table[pos] = value;
}
int main(void){
        long long idx;
        std::vector<int> v(5);
        std::cin >> idx;
        insert_table(v, idx, 0x41414141);
}
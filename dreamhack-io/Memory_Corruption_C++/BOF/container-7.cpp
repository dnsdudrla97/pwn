#include <vector>
#include <iostream>

void insert_table(std::vector<int> &table, long long pos, int value) {
    if (pos >= (long long)table.size()) {
        std::cout << "OVERFLOW!" << std::endl;
        return;
    }
    table[pos] = value;
}

int main() {
    long long idx;
    std::vector<int> v(5);
    std::cin >> idx;

    insert_table(v, idx, 0x41414141);
}


#include <algorithm>
#include <vector>
int main() {
	std::vector<int> v(10);
	std::fill_n(v.begin(), 10, 0x42);
}
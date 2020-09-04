#include <cstdlib>
#include <string>
#include <iostream>
int main()
{
    std::string tmp(std::getenv("TMP"));
    if (!tmp.empty())
    {
        std::cout << tmp << std::endl;
    }
}
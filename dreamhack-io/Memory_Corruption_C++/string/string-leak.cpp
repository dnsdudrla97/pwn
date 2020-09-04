#include <iostream>
#include <fstream>

int main()
{
    char buffer[32];
    std::ifstream is("test.txt", std::ifstream::binary); // test.txt have 32bytes of "a"

    try
    {
        is.read(buffer, sizeof(buffer));
    }
    catch (std::ios_base::failure &e)
    {
        std::cout << "Error handling" << std::endl;
    }
    std::cout << buffer << std::endl;
}
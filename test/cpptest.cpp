#include <string>
#include <iostream>

int main()
{
    std::string s = "A0";
    int a = std::stoi(s,0,16);
    std::cout << a;
}

#include <windows.h>
#include "Numpp.hpp"
using namespace dsl;

int main() {
    SetConsoleOutputCP(65001);
    BigNum a, b;
    std::cout << "输入整数 a :\n>>> ";
    std::cin >> a;
    std::cout << "输入整数 b :\n>>> ";
    std::cin >> b;

    std::cout << "a + b = " << a + b << '\n'
            << "a - b = " << a - b << '\n'
            << "a * b = " << a * b << '\n';
    return 0;
}
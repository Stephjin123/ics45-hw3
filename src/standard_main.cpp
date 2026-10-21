#include "string.hpp"
#include <iostream>

int main() {
    String s("hello");
    std::cout << s << " " << s.reverse() << " " << s.size() << std::endl;
    return 0;
}

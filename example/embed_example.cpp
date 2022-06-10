#include <iostream>
#include <lorem.txt.hpp>

int main() {
    std::cout << "-------------- BEGIN EMBEDDED CONTENT --------------\n";
    lorem_txt_dump(std::cout);
    std::cout << "\n-------------- END EMBEDDED CONTENT --------------\n";
}
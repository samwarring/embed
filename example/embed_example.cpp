#include <iostream>
#include <lorem.txt.hpp>

int main() {
    std::cout << "-------------- BEGIN EMBEDDED CONTENT --------------\n";
    for (std::size_t i = 0; i < lorem_txt_num_chunks; ++i) {
        std::cout.write(lorem_txt_chunks[i], lorem_txt_chunk_sizes[i]);
    }
    std::cout << "-------------- END EMBEDDED CONTENT --------------\n";
}
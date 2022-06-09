#include <iostream>
#include <lorem.hpp>
#include <string_view>

int main() {
    std::cout << "-------------- BEGIN EMBEDDED CONTENT --------------\n";
    for (std::size_t i = 0; i < example::lorem_num_chunks; ++i) {
        auto chunk =
            std::string_view(example::lorem_chunks[i], example::lorem_chunk_sizes[i]);
        std::cout << chunk;
    }
    std::cout << "-------------- END EMBEDDED CONTENT --------------\n";
}
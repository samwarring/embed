#include "test.txt.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: embed_test <path to test.txt>\n";
        return 1;
    }
    std::ifstream actual_file(argv[1]);
    if (!actual_file) {
        std::cerr << "error: failed to open test file " << argv[1] << '\n';
        return 1;
    }
    if (test_txt_num_chunks < 2) {
        std::cerr << "error: embedded file is only one chunk. Use a bigger file, or "
                     "smaller chunk size\n";
        return 1;
    }

    // Compare contents of actual file and embedded file
    std::stringstream embedded_file;
    test_txt_dump(embedded_file);
    for (std::size_t i = 0; actual_file && embedded_file; ++i) {
        char actual_ch, embedded_ch;
        actual_file.read(&actual_ch, 1);
        embedded_file.read(&embedded_ch, 1);
        if (actual_ch != embedded_ch) {
            std::cerr << "error: actual vs embedded contents differ at position " << i
                      << ". actual=\"" << actual_ch << "\", embedded=\"" << embedded_ch
                      << "\"\n";
            return 1;
        }
    }
    if (actual_file) {
        std::cerr << "error: actual file is longer than embedded file\n";
        return 1;
    }
    if (embedded_file) {
        std::cerr << "error: embedded file is longer than actual file\n";
        return 1;
    }

    // Files were identical
    return 0;
}
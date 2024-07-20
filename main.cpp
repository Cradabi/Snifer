#include <iostream>
#include "mac_address_counter.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string result = process_file(argv[1]);
    std::cout << result;

    return 0;
}

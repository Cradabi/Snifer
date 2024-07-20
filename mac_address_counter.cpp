#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

std::string delete_spaces(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end ? std::string(start, end) : std::string());
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string process_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "Error: Could not open file " + filename;
    }

    std::unordered_map<std::string, int> addressCount;
    std::string line;

    while (std::getline(file, line)) {
        auto columns = split(line, ',');
        for (const auto& str : columns) {
            if (str.find("RA=") != std::string::npos || str.find("RA/") != std::string::npos || str.find("TA=") != std::string::npos || str.find("TA/") != std::string::npos || str.find("SA=") != std::string::npos || str.find("SA/") != std::string::npos) {
                auto parts = split(str, '=');
                if (parts.size() == 2) {
                    std::string macAddress = delete_spaces(parts[1]);
                    addressCount[macAddress]++;
                }
            }
        }
    }

    file.close();

    std::vector<std::pair<std::string, int>> sortedAddresses(addressCount.begin(), addressCount.end());
    std::sort(sortedAddresses.begin(), sortedAddresses.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::stringstream result;
    for (const auto& pair : sortedAddresses) {
        result << pair.first << ": " << pair.second << "\n";
    }

    return result.str();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string result = process_file(argv[1]);
    std::cout << result;

    return 0;
}

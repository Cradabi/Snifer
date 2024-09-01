#include "mac_address_counter.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include <iostream>  // Добавьте этот заголовок

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
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + filename);
        }

        std::unordered_map<std::string, int> addressCount;
        std::string line;

        while (std::getline(file, line)) {
            try {
                auto columns = split(line, ',');
                for (const auto& str : columns) {
                    if (str.find("RA=") != std::string::npos || str.find("RA/") != std::string::npos || str.find("TA=") != std::string::npos || str.find("TA/") != std::string::npos || str.find("SA=") != std::string::npos || str.find("SA/") != std::string::npos) {
                        auto parts = split(str, '=');
                        if (parts.size() == 2) {
                            std::string macAddress = parts[1];
                            macAddress.erase(std::remove_if(macAddress.begin(), macAddress.end(),
                                             [](unsigned char ch) { return std::isspace(ch); }),
                                             macAddress.end());
                            addressCount[macAddress]++;
                        }
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Error processing line: " << e.what() << std::endl;
                // continue удален, так как он здесь не нужен
            }
        }

        file.close();

        std::vector<std::pair<std::string, int>> sortedAddresses(addressCount.begin(), addressCount.end());

        try {
            std::sort(sortedAddresses.begin(), sortedAddresses.end(),
                      [](const auto& a, const auto& b) { return a.second > b.second; });
        } catch (const std::exception& e) {
            std::cerr << "Error during sorting: " << e.what() << std::endl;
        }

        std::stringstream result;
        for (const auto& pair : sortedAddresses) {
            result << pair.first << ": " << pair.second << "\n";
        }

        return result.str();

    } catch (const std::exception& e) {
        return "Error: " + std::string(e.what());
    }
}

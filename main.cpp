#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <regex>


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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Пожалуйста, укажите текст в качестве аргумента командной строки." << std::endl;
        return 0;
    }
    std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << std::endl;
        return 1;
    }

    std::unordered_map<std::string, int> addressCount;
    std::string line;

    while (std::getline(file, line)) {
        auto columns = split(line, ',');
        for (const auto& str : columns) {
            if (str.find("RA=") != std::string::npos || str.find("RA/") != std::string::npos || str.find("TA=") != std::string::npos || str.find("TA/") != std::string::npos || str.find("SA=") != std::string::npos || str.find("SA/") != std::string::npos) {
                auto parts = split(str, '=');
                if (parts.size() == 2) {
                    std::string macAddress = parts[1];
                    // Удаляем возможные пробелы в начале и конце MAC-адреса
                    macAddress = delete_spaces(macAddress);
                    if (addressCount.find(macAddress) != addressCount.end()) {
                        addressCount[macAddress]++;
                    } else {
                        addressCount[macAddress] = 0;
                    }
                }
            }
        }
    }

    file.close();

    std::vector<std::pair<std::string, int>> sortedAddresses;
    for (const auto& item : addressCount) {
        sortedAddresses.push_back(item);
    }
    std::sort(sortedAddresses.begin(), sortedAddresses.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    // Вывод результатов
    for (const auto& pair : sortedAddresses) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}

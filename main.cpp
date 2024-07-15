#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <regex>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

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

TEST(MacAddressCounterTest, CorrectOutput) {
    std::string testFilename = "frames_parser.log";

    std::string result = process_file(testFilename);

    std::string expected = "b8:69:f4:7a:a5:ac: 15235\n"
                           "34:1c:f0:d3:40:a2: 5812\n"
                           "34:1c:f0:d2:78:5a: 5307\n"
                           "00:0c:29:65:08:ee: 3713\n"
                           "84:c5:a6:07:38:66: 124\n"
                           "6e:52:4e:5f:f9:eb: 107\n"
                           "4a:5f:99:ae:ea:99: 98\n"
                           "ff:ff:ff:ff:ff:ff: 98\n"
                           "b8:69:f4:7a:a5:93: 68\n"
                           "52:ff:20:52:16:9a: 14\n"
                           "70:c9:32:1b:54:e2: 13\n"
                           "80:b6:55:60:6f:58: 4\n"
                           "c8:7f:54:28:74:ac: 3\n";

    if (result != expected) {
        FAIL() << "Expected output:\n" << expected << "\nActual output:\n" << result;
    }

    std::remove(testFilename.c_str());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

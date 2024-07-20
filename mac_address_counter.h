#pragma once
#include <string>
#include <vector>

std::string delete_spaces(const std::string& str);
std::vector<std::string> split(const std::string& s, char delimiter);
std::string process_file(const std::string& filename);

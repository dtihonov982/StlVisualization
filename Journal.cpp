#include "Journal.h"
#include <cassert>
#include <sstream>

std::vector<std::string> split(const std::string& input, char delim) {
    std::istringstream iss{input};
    std::string token;
    std::vector<std::string> result;
    while(std::getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

Journal readJournal(std::istream& is, char delim) {
    Journal result;
    for (std::string line; std::getline(is, line);) {
        auto entry = split(line, delim);
        result.push_back(std::move(entry));
    }
    return result;
}

std::vector<int> loadDataFromDump(const std::string& dump, char delim) {
    auto tokens = split(dump, delim);
    std::vector<int> result;
    for (auto& token: tokens) {
        result.push_back(std::stoi(token));
    }
    return result;
}

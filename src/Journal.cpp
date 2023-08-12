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

std::vector<Action> readJournal(std::istream& is, char delim) {
    std::vector<Action> journal;
    for (std::string line; std::getline(is, line);) {
        auto entry = split(line, delim);
        int pos = std::stoi(entry[1]);
        if (entry[0] == "access") {
            journal.push_back({Action::MARK, pos, 0});
        }
        else if (entry[0] == "write") {
            int value = std::stoi(entry[2]);
            journal.push_back({Action::WRITE, pos, value});
        }
    }
    return journal;
}

std::vector<int> loadDataFromDump(const std::string& dump, char delim) {
    auto tokens = split(dump, delim);
    std::vector<int> result;
    for (auto& token: tokens) {
        result.push_back(std::stoi(token));
    }
    return result;
}

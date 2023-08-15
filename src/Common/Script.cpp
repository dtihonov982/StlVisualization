#include "Script.h"
#include <cassert>
#include <sstream>
#include "Exception.h"

std::vector<std::string> split(const std::string& input, char delim) {
    std::istringstream iss{input};
    std::string token;
    std::vector<std::string> result;
    while(std::getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

std::vector<Action> readScript(std::istream& is, char delim) {
    std::vector<Action> script;
    for (std::string line; std::getline(is, line);) {
        auto entry = split(line, delim);
        int rawPos; 
        try {
            rawPos = std::stoi(entry[2]);
        }
        catch (const std::invalid_argument& ex) {
            throw Exception("Error conversion to int: ", entry[2]);
        }
        if (rawPos < 0) {
            throw Exception("Negative position.");
        }
        size_t pos = static_cast<size_t>(rawPos);
        if (entry[1] == "access") {
            script.push_back({Action::ACCESS, pos, 0});
        }
        else if (entry[1] == "write") {
            int value = std::stoi(entry[3]);
            script.push_back({Action::WRITE, pos, value});
        }
        else {
            throw Exception("Unknow token ", entry[1], " in line ", line);
        }
    }
    return script;
}

std::vector<int> loadDataFromDump(const std::string& dump, char delim) {
    auto tokens = split(dump, delim);
    std::vector<int> result;
    for (auto& token: tokens) {
        result.push_back(std::stoi(token));
    }
    return result;
}

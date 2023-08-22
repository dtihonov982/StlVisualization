#ifndef SCRIPT_H
#define SCRIPT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>

struct Record {
    std::string info;
    int speed = 100; // ms per frame / ns
    std::vector<Action> script;
};

struct Action {
    enum Type { ACCESS, WRITE };

    uint64_t timePoint;
    Type type;
    size_t pos;
    int value;

    std::string toString() const;
    static Action loadFromString(std::string_view str);
};

using Script = std::vector<Action>;
using Actions = std::vector<Action>;

std::vector<std::string> split(std::string_view input, char delim);

size_t tryToReadSizeT(std::string_view str);

std::vector<Action> readScript(std::istream& is, char delim);

std::vector<int> loadDataFromDump(const std::string& dump, char delim);

template<typename... Args>
std::string concate(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

#endif //SCRIPT_H

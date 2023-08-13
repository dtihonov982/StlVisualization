#ifndef SCRIPT_H
#define SCRIPT_H

#include <iostream>
#include <string>
#include <vector>

struct Action {
    enum Type { ACCESS, WRITE };
    Type type;
    size_t pos;
    int value;
};

using Script = std::vector<Action>;

std::vector<std::string> split(const std::string& input, char delim);

std::vector<Action> readScript(std::istream& is, char delim);

std::vector<int> loadDataFromDump(const std::string& dump, char delim);

#endif //SCRIPT_H

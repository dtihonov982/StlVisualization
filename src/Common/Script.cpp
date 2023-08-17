#include "Script.h"
#include <cassert>
#include <sstream>
#include <string>

#include "Exception.h"

std::string Action::toString() const {
    std::ostringstream oss;
    oss << timePoint << ",";
    switch (type) {
        case ACCESS:
            oss << "access,";
            oss << pos;
            break;
        case WRITE:
            oss << "write,";
            oss << pos << ",";
            oss << value;
            break;
        default:
            throw Exception("Bad Action::Type.");
    }
    oss << "\n";
    return oss.str();
}

Action Action::loadFromString(std::string_view str) {
    if (str.empty())
        throw Exception("Can not load Action from empty string.");
    auto error = concate("Can not load Action from \"", str, "\".");
    auto tokens = split(str, ',');
    if (tokens.size() < 3)
        throw Exception(error);
    Action action;

    action.timePoint = tryToReadSizeT(tokens[0]);

    if (tokens[1] == "access") {
        action.type = ACCESS;
    }
    else if (tokens[1] == "write") {
        action.type = WRITE;
    }
    else {
        throw Exception(error);
    }

    action.pos = tryToReadSizeT(tokens[2]);

    if (action.type == WRITE) {
        if (tokens.size() < 3)
            throw Exception(error);
        action.value = tryToReadSizeT(tokens[3]);
    }

    return action;

}

std::vector<std::string> split(std::string_view input, char delim) {
    std::istringstream iss{input.data()};
    std::string token;
    std::vector<std::string> result;
    while(std::getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

size_t tryToReadSizeT(std::string_view str) {
        long raw;
        try {
            raw = std::stol(str.data());
        }
        catch (std::invalid_argument& ex) {
            throw Exception("Can't convert ", str, " to size_t.");
        }
        if (raw < 0) 
            throw Exception(str, " can't be size_t because it's < 0.");
        return static_cast<size_t>(raw);
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
            script.push_back({0, Action::ACCESS, pos, 0});
        }
        else if (entry[1] == "write") {
            int value = std::stoi(entry[3]);
            script.push_back({0, Action::WRITE, pos, value});
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

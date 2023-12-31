#include <cassert>
#include <sstream>
#include <string>
#include <fstream>

#include "Exception.h"
#include "Common.h"
#include "Record.h"

size_t tryToReadSizeT(std::string_view str);
std::vector<std::string> split(std::string_view input, char delim);
std::vector<int> loadDataFromDump(const std::string& dump, char delim);
std::vector<Action> readScript(std::istream& is, char delim);

template<typename... Args>
std::string concate(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

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
    return oss.str();
}

void Action::toJSON(json& j) const {
    j["timePoint"] = timePoint;
    j["pos"] = pos;
    j["type"] = type == ACCESS ? "access" : "write";
    if (type == WRITE) 
        j["value"] = value;

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

Action Action::fromJSON(const json& j) {
    Action a;
    a.timePoint = j["timePoint"];
    a.pos = j["pos"];
    std::string typeName = j["type"];
    a.type = typeName == "access" ? ACCESS : WRITE;
    if (a.type == WRITE)
        a.value = j["value"];
    return a;
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
        Action curr = Action::loadFromString(line);
        script.push_back(curr);
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

void Record::save(const std::string& path) {
    std::ofstream file(path);
    if (!file)
        throw Exception("Can't open file ", path);
    json j;
    j["info"] = info;
    j["data"] = data;
    j["resultData"] = resultData;
    json scriptArray = json::array();
    for (const auto& action: script) {
        json tmp;
        action.toJSON(tmp);
        scriptArray.push_back(tmp);
    }
    j["script"] = scriptArray;
    file << j.dump(4);
}

Record Record::load(std::string_view filename) {
    std::ifstream file{filename.data()};
    if (!file)
        throw Exception("Can't open file ", filename);
    json j;
    file >> j;

    Record rec;
    rec.info = j["info"];
    rec.data = j["data"].template get<std::vector<int>>();
    rec.resultData = j["resultData"].template get<std::vector<int>>();
    for (auto& e: j["script"]) {
        Action a = Action::fromJSON(e);
        rec.script.push_back(a);
    } 

    return rec;
}
        

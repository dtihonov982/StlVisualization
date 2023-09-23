#ifndef SCRIPT_H
#define SCRIPT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <filesystem>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

struct Action {
    enum Type { ACCESS, WRITE };

    uint64_t timePoint;
    Type type;
    size_t pos;
    int value;

    std::string toString() const;
    void toJSON(json& j) const;
    static Action loadFromString(std::string_view str);
};

using Script = std::vector<Action>;

struct Record {
    Record() {}
    Record(std::string_view info_, const std::vector<int>& data_):
    info(info_), data(data_) {}

    std::string info;
    std::vector<int> data;
    std::vector<Action> script;

    void save(const std::string& path);
    static Record load(std::string_view filename);
};

#endif //SCRIPT_H

#include <fstream>

#include "Playlist.h"
#include "Common/Exception.h"

Playlist::Entry Playlist::Entry::loadFromString(const std::string& input) {
    std::istringstream is{input};
    Entry entry;
    is >> entry.delayRatio;
    if (!is)
        throw Exception("Can't read a delay ratio from stream.");
    char unnecessaryDelim;
    is.get(unnecessaryDelim);
    std::string pathStr;
    while (std::getline(is, pathStr, ',')) {
        entry.files.emplace_back(pathStr);
    }
    return entry;
}

Playlist Playlist::loadFromFile(const fs::path& path) {
    std::ifstream file{path};
    if (!file)
        throw Exception("Can't open the file ", path);
    Playlist list;
    std::string line;
    while (std::getline(file, line)) {
        list.entries_.emplace_back(Entry::loadFromString(line));
    }
    return list;
}

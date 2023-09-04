#pragma once
#include <vector>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Playlist {
public:
    struct Entry {
        float delayRatio;
        std::vector<fs::path> files;

        static Entry loadFromString(const std::string& input);
    };
    static Playlist loadFromFile(const fs::path& path);
private:
    std::vector<Entry> entries_;
};
    

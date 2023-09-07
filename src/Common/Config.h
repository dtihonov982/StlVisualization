#ifndef CONFIG_H
#define CONFIG_H

#include <regex>
#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <filesystem>
#include <istream>

#include "TEMap.h"

class Config: public TEMap {
public:
	Config(std::filesystem::path filename);
	void readStream(std::istream& istream);	
    static const std::vector<std::regex> regexes;
    static std::smatch parseString(const std::string& input);
};

#endif // CONFIG_H

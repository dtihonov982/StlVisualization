#include "Common/Config.h"
#include "Common/Exception.h"

Config::Config(std::filesystem::path filename) {
	std::ifstream istream{filename};  
	readStream(istream);
}

const std::vector<std::regex> Config::regexes{ 
    std::regex{"(\\w+)\\s*=\\s*(\\w+)\\s*"}, 
    std::regex{"(\\w+)\\s*=\\s*\"([\\s\\w]*)\"\\s*"}
};

std::smatch Config::parseString(const std::string& input) {
    std::smatch matches;
    for (std::regex re: regexes) {
        if (std::regex_match(input, matches, re)) {
            break;
        }
    }
    return matches;
}

void ltrim(std::string& str) {
    auto it = std::find_if_not(str.begin(), str.end(), [](char c) { return c == ' '; });
    str.erase(str.begin(), it);
}

void rtrim(std::string& str) {
    auto it = std::find_if_not(str.rbegin(), str.rend(), [](char c) { return c == ' '; });
    // ??? why not -1
    str.erase(it.base(), str.end());
}

void trim(std::string& str) {
    ltrim(str);
    rtrim(str);
}

std::pair<std::string, std::string> getNameAndValue(const std::string& line) {
    auto n = line.find("=");
    if (n == std::string::npos)
        throw Exception("Can not parse line: ", line);
    std::string param(line.begin(), line.begin() + n);
    rtrim(param);
    std::string value(line.begin() + n + 1, line.end());
    ltrim(value);
    return {param, value};
}

void Config::readStream(std::istream& istream) {
    std::string line;
    
    std::regex emptyString{"\\s*"};
    std::regex decimalInt{"\\d+"};
    std::regex hexInt{"0[xX][0-9a-fA-F]+"};

    std::smatch expr;
    std::string name;
    std::string value;
    while (std::getline(istream, line)) {

        line.erase(std::find(line.begin(), line.end(), ';'), line.end());
        trim(line);
        if (line.empty())
            continue;

        auto [name, value] = getNameAndValue(line);
        if (std::regex_match(value, decimalInt)) {
            TEMap::insert<int>(name, std::stoi(value));
        }
        else if (std::regex_match(value, hexInt)) {
            TEMap::insert<long long>(name, std::stoll(value, nullptr, 16));
        }
        else {
            TEMap::insert<std::string>(name, value);
        }
	}
}	

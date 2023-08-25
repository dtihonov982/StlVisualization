#include "Common/Config.h"

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

        if(std::regex_match(line, emptyString))
            continue;

        expr = Config::parseString(line);

        assert(expr.size() >= 3);

        name = expr[1];
        value = expr[2];
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

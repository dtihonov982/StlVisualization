#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <string_view>

using Entry = std::vector<std::string>;
using Journal = std::vector<Entry>;

std::vector<std::string> split(const std::string& input, char delim) {
    std::istringstream iss{input};
    std::string token;
    std::vector<std::string> result;
    while(std::getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

Journal readJournal(std::istream& is, char delim) {
    Journal result;
    for (std::string line; std::getline(is, line);) {
        auto entry = split(line, delim);
        result.push_back(std::move(entry));
    }
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (const auto& x: vec) {
        os << x << ", ";
    }
    return os;
}

std::vector<int> loadDataFromDump(const std::string& dump, char delim) {
    auto tokens = split(dump, delim);
    std::vector<int> result;
    for (auto& token: tokens) {
        result.push_back(std::stoi(token));
    }
    return result;
}

int main() {
    std::stringstream ss( \
R"(std::find(..., 3)
1,2,3,4,5
r,0
w,2,10)"
    );
    std::cout << ss.str() << "\n" << "\n";

    std::string title;
    std::getline(ss, title);
    std::cout << title << '\n';


    std::string strData;
    std::getline(ss, strData);
    auto data = loadDataFromDump(strData, ',');
    std::cout << data << '\n';


    auto journal = readJournal(ss, ',');
    std::cout << journal << '\n';

    for (auto& entry: journal) {
        if (entry[0] == "r") {
            std::cout << "reading at " << entry[1] << " position\n";
        }
        else if (entry[0] == "w") {
            std::cout << "writing a " << entry[2] << " in " << entry[1] << " position\n";
        }
    }

    return 0;
}

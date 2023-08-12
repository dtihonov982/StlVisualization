#ifndef JOURNAL_H
#define JOURNAL_H

#include <iostream>
#include <string>
#include <vector>

struct Action {
    enum Type { MARK, WRITE };
    Type type;
    size_t pos;
    int value;
};

using Entry = Action;
using Journal = std::vector<Action>;

std::vector<std::string> split(const std::string& input, char delim);

std::vector<Action> readJournal(std::istream& is, char delim);

std::vector<int> loadDataFromDump(const std::string& dump, char delim);

#endif //JOURNAL_H

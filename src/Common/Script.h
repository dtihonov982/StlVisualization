#ifndef JOURNAL_H
#define JOURNAL_H

#include <iostream>
#include <string>
#include <vector>

using Entry = std::vector<std::string>;
using Journal = std::vector<Entry>;

std::vector<std::string> split(const std::string& input, char delim);

Journal readJournal(std::istream& is, char delim);

std::vector<int> loadDataFromDump(const std::string& dump, char delim);

#endif //JOURNAL_H

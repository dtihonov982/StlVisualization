#include <algorithm>
#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <fstream>
#include <random>

#include "AccessLogger.h"
#include "NotifyingIterator.h"
#include "Common.h"

int main() {
    std::vector<int> data(10);
    std::iota(data.rbegin(), data.rend(), 0);

    std::ofstream file{"log/AccesLoggerTest.txt"};
    file << "title\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), logger);

    std::find(begin_, end_, 1000);

    logger.finalize();


    return 0;
}

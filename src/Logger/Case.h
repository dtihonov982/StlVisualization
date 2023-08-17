#ifndef CASE_H
#define CASE_H

#include <vector>
#include <chrono>
#include <string_view>
#include <fstream>
#include <utility>

#include "Exception.h"
#include "AccessLogger.h"

using OriginalIterator = std::vector<int>::iterator;
using NIter = NotifyingIterator<OriginalIterator>;

class Case {
public:
    using Container = std::vector<int>;
    Case(const Container& data, 
         std::string_view name, 
         const AccessLogger<Container>::time_point& startPoint = std::chrono::high_resolution_clock::now())
    : data_(data)
    , name_(name)
    , info_(name)
    , file_(getPath(name))
    , logger_(data_, file_, startPoint) {
        if (!file_)
            throw Exception("Can't open file ", getPath(name));
        file_ << info_ << "\n";
        file_ << data_ << "\n";
    }

    //return two NotifyingIterators to begin and end of the data
    std::pair<NIter, NIter> getIterators() {
        auto begin = NotifyingIterator(data_.begin(), data_.begin(), logger_);
        auto end = NotifyingIterator(data_.begin(), data_.end(), logger_);
        return {begin, end};
    }

    void finalize() {
        logger_.finalize();
    }

    static std::string getPath(std::string_view algoName) {
        std::string path{"logs/"};
        path += algoName;
        path += ".txt";
        return path;
    }

private:
    Container data_;
    std::string_view name_;
    std::string_view info_;
    std::ofstream file_;
    AccessLogger<Container> logger_;
};

#endif //CASE_H

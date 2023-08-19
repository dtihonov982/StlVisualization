#ifndef RECORDER_H
#define RECORDER_H

#include <vector>
#include <chrono>
#include <string_view>
#include <fstream>
#include <utility>
#include <memory>

#include "Common/Exception.h"
#include "Logger/EventInterpreter.h"

using OriginalIterator = std::vector<int>::iterator;
using NIter = NotifyingIterator<OriginalIterator>;

//Create recording and save it in file
class Recorder {
public:
    using Container = std::vector<int>;

    Recorder(std::string_view name, const Container& data)
    : data_(data)
    , name_(name)
    , info_(name)
    , interpreter_(data_) {
    }

    //Many recorders may have one stopwatch for synchronized logging many data algorithms
    Recorder(std::string_view name,
             const Container& data, 
             const std::shared_ptr<Stopwatch>& stopwatch_)
    : data_(data)
    , name_(name)
    , info_(name)
    , interpreter_(data_, stopwatch_) {
    }

    //return two NotifyingIterators to begin and end of the data
    std::pair<NIter, NIter> getIterators() {
        auto begin = NotifyingIterator(data_.begin(), data_.begin(), interpreter_);
        auto end = NotifyingIterator(data_.begin(), data_.end(), interpreter_);
        return {begin, end};
    }

    void save() {
        std::ofstream file(getPath(name_));
        if (!file)
            throw Exception("Can't open file ", getPath(name_));
        file << info_ << "\n";
        file << data_ << "\n";
        for (const auto& action: interpreter_.getScript()) {
            file << action.toString() << "\n";
        }
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
    EventInterpreter<Container> interpreter_;
};

#endif //RECORDER_H

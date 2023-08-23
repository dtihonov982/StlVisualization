#ifndef RECORDER_H
#define RECORDER_H

#include <vector>
#include <chrono>
#include <string_view>
#include <fstream>
#include <utility>
#include <memory>
#include <filesystem>

#include "Common/Exception.h"
#include "Logger/EventInterpreter.h"

using OriginalIterator = std::vector<int>::iterator;
using NIter = NotifyingIterator<OriginalIterator>;

namespace fs = std::filesystem;

//Create recording and save it in file
class Recorder {
public:
    using Container = std::vector<int>;

    Recorder(const std::string& name, const Container& data)
    : data_(std::make_shared<Container>(data))
    , dataOriginal_(data)
    , name_(name)
    , info_(name)
    , interpreter_(data_) {
    }

    //Many recorders may have one stopwatch for synchronized logging many data algorithms
    Recorder(std::string_view name,
             const Container& data, 
             const std::shared_ptr<Stopwatch>& stopwatch_)
    : data_(std::make_shared<Container>(data))
    , dataOriginal_(data)
    , name_(name)
    , info_(name)
    , interpreter_(data_, stopwatch_) {
    }

    //return two NotifyingIterators to begin and end of the data
    std::pair<NIter, NIter> getIterators() {
        auto begin = NotifyingIterator(data_->begin(), data_->begin(), interpreter_);
        auto end = NotifyingIterator(data_->begin(), data_->end(), interpreter_);
        return {begin, end};
    }

    void save() {
        auto path = getPathForSaving(name_);
        std::ofstream file(path);
        if (!file)
            throw Exception("Can't open file ", path);
        file << info_ << "\n";
        file << dataOriginal_ << "\n";
        for (const auto& action: interpreter_.getScript()) {
            file << action.toString() << "\n";
        }
    }

    static std::string getPathForSaving(std::string_view algoName) {
        fs::path logs{"logs"};
        fs::create_directory(logs);
        fs::path filename{algoName.data()};
        filename = logs / filename;
        fs::path extension{".txt"};
        filename += extension;
        return filename;
    }

private:
    std::shared_ptr<Container> data_;
    const Container dataOriginal_;
    std::string name_;
    std::string info_;
    EventInterpreter<Container> interpreter_;
};

#endif //RECORDER_H

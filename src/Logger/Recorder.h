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
#include "Common/Common.h"
#include "Logger/EventInterpreter.h"
#include "Logger/NotifyingIterator.h"

template <typename Container>
using NIter = NotifyingIterator<typename Container::iterator>;

namespace fs = std::filesystem;

//Create recording and save it in file
template <typename Container>
class Recorder {
public:

    Recorder(const std::string& name, const Container& data)
    : data_(std::make_shared<Container>(data))
    , record_(name, data)
    , name_(name)
    , interpreter_(data_) {
    }

    //Many recorders may have one stopwatch for synchronized logging many data algorithms
    Recorder(std::string_view name,
             const Container& data, 
             const std::shared_ptr<Stopwatch>& stopwatch_)
    : data_(std::make_shared<Container>(data))
    , record_(name, data)
    , name_(name)
    , interpreter_(data_, stopwatch_) {
    }

    Recorder(std::string_view name,
             std::string_view info,
             const Container& data, 
             const std::shared_ptr<Stopwatch>& stopwatch_)
    : data_(std::make_shared<Container>(data))
    , record_(info, data)
    , name_(name)
    , interpreter_(data_, stopwatch_) {
    }

    //return two NotifyingIterators to begin and end of the data
    std::pair<NIter<Container>, NIter<Container>> 
    getIterators() {
        auto begin = NotifyingIterator(data_->begin(), data_->begin(), interpreter_);
        auto end = NotifyingIterator(data_->begin(), data_->end(), interpreter_);
        return {begin, end};
    }

    void save() {
        auto path = getPathForSaving(name_);
        record_.script = interpreter_.getScript();
        record_.resultData = *data_;
        record_.save(path);
    }

    void pause() { interpreter_.pauseRecording(); }
    void resume() { interpreter_.resumeRecording(); }

    static std::string getPathForSaving(std::string_view algoName) {
        fs::path logs{"logs"};
        fs::create_directory(logs);
        fs::path filename{algoName.data()};
        filename = logs / filename;
        fs::path extension{".json"};
        filename += extension;
        return filename;
    }

private:
    std::shared_ptr<Container> data_;
    Record record_;
    std::string name_;
    EventInterpreter<Container> interpreter_;
};

#endif //RECORDER_H

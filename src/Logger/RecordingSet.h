#ifndef RECORDINGSET_H
#define RECORDINGSET_H

#include <unordered_map>

#include "Logger/Recorder.h"
#include "Logger/Stopwatch.h"

class RecordingSet {
public:
    using Container = std::vector<int>;

    std::pair<NIter, NIter> add(const std::string& name, const Container& data) {
        recorders_.emplace(name, Recorder {name, data, synchTime_});
        auto it = recorders_.find(name);
        if (it != recorders_.end()) {
            return it->second.getIterators();
        }
        else {
            throw Exception("Adding ", name, " in recording set error.");
        }
    }

    void startStopwatch() {
        synchTime_->start();
    }

     std::pair<NIter, NIter> getIterators(const std::string& name) {
        auto it = recorders_.find(name);
        if (it != recorders_.end()) {
            return it->second.getIterators();
        }
        else {
            throw Exception("There is no recorder with name ", name, " in recording set.");
        }
    }

    void save() {
        for (auto& [name, rec]: recorders_) 
            rec.save();
    }

private:
    std::unordered_map<std::string, Recorder> recorders_;
    std::shared_ptr<Stopwatch> synchTime_ = std::make_shared<Stopwatch>();
};


#endif //RECORDINGSET_H

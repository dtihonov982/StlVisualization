#ifndef RECORDINGSET_H
#define RECORDINGSET_H

#include <unordered_map>

#include "Logger/Recorder.h"
#include "Logger/Stopwatch.h"

// RecordingSet organizes recording of algorithms that gets many data as input (std::merge etc.).
class RecordingSet {
public:
    using Container = std::vector<int>;

    std::pair<NIter, NIter> add(const std::string& name, const Container& data) {
        recorders_.emplace(name, Recorder {name, data, synchTime_});
        return getIterators(name);
    }

    // Call this method before an algorithm execution to minimize start delay in a record playing.
    void runStopwatch() {
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

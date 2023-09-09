#ifndef RECORDINGSET_H
#define RECORDINGSET_H

#include <unordered_map>

#include "Logger/Recorder.h"
#include "Logger/Stopwatch.h"

// RecordingSet organizes recording of algorithms that gets many data as input (std::merge etc.).
template <typename Container>
class RecordingSet {
public:

    std::pair<NIter<Container>, NIter<Container>> 
    add(const std::string& name, const Container& data) {
        recorders_.emplace(name, Recorder {name, data, synchTime_});
        return getIterators(name);
    }

    std::pair<NIter<Container>, NIter<Container>>
    add(const std::string& name, const std::string& info, const Container& data) {
        recorders_.emplace(name, Recorder {name, info, data, synchTime_});
        return getIterators(name);
    }

    // Call this method before an algorithm execution to minimize start delay in a record playing.
    void runStopwatch() {
        synchTime_->start();
    }

    std::pair<NIter<Container>, NIter<Container>> 
    getIterators(const std::string& name) {
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

    void pause() {
        for (auto& [name, rec]: recorders_) 
            rec.pause();
    }

    void resume() {
        for (auto& [name, rec]: recorders_) 
            rec.resume();
    }

private:
    std::unordered_map<std::string, Recorder<Container>> recorders_;
    std::shared_ptr<Stopwatch> synchTime_ = std::make_shared<Stopwatch>();
};


#endif //RECORDINGSET_H

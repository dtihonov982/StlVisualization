#include <vector>
#include <iostream>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"

int main() {
    std::vector<int> data(10);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("RecordingPauseTest", "RecordingPauseTest", data);
        set.runStopwatch();

        // access,0
        *f1;
        ++f1;
        set.pause();
        // nothing
        *f1;
        ++f1;
        set.resume();
        // access,2
        *f1;
        ++f1;
        set.pause();
        // nothing
        *f1;
        ++f1;
        set.resume();

        set.save();
    }
    catch (const Exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}


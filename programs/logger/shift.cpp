#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, shift) {
    std::vector<int> data(30);
    std::iota(data.begin(), data.end(), 1);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("shift_left", "std::shift_left", data);
        set.runStopwatch();
        std::shift_left(f1, l1, data.size() / 2);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("shift_right", "std::shift_right", data);
        set.runStopwatch();
        std::shift_right(f1, l1, data.size() / 2);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

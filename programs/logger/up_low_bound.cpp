#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, bound) {
    std::vector<int> data = getRandVector(30, 1, 300);
    std::vector<int> equal(20, 150);
    data.insert(data.end(), equal.begin(), equal.end());
    std::sort(data.begin(), data.end());

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("lower_bound", "std::lower_bound", data);
        set.runStopwatch();
        std::lower_bound(f1, l1, 150);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("upper_bound", "std::upper_bound", data);
        set.runStopwatch();
        std::upper_bound(f1, l1, 150);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("equal_range", "std::equal_range", data);
        set.runStopwatch();
        std::equal_range(f1, l1, 150);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, rotate) {
    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 1);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("rotate", "std::rotate", data);
        set.runStopwatch();
        std::rotate(f1, f1 + 100/3, l1);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


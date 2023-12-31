#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, shuffle) {
    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 1);
    std::random_device dev;
    std::mt19937 rng(dev());

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("shuffle", "std::shuffle", data);
        set.runStopwatch();
        std::shuffle(f1, l1, rng);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, remove_if) {
    std::vector<int> data = getRandVector(30, 1, 100);
    int value = 300;
    std::vector<int> addition(20, value);
    data.insert(data.end(), addition.begin(), addition.end());
    std::random_device dev;
    std::mt19937 rng(dev());
    std::shuffle(data.begin(), data.end(), rng);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("remove_if", "std::remove_if", data);
        set.runStopwatch();
        std::remove_if(f1, l1, [&] (int x) { return x == value; });
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


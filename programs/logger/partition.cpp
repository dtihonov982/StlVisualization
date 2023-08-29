#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, partition) {
    std::vector<int> data = getRandVector(80, 1, 1000);
    int partitionValue = 500;
    std::vector<int> addition(20, partitionValue);
    data.insert(data.end(), addition.begin(), addition.end());
    std::random_device dev;
    std::mt19937 rng(dev());
    std::shuffle(data.begin(), data.end(), rng);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("partition", "std::partition P(x) = x == value", data);
        set.runStopwatch();
        std::partition(f1, l1, [partitionValue] (int x) { return x == partitionValue; });
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


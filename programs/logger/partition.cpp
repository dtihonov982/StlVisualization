#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, partition) {
    int count = 80;
    std::vector<int> data = getRandVector(count, 1, 1000);
    int partitionValue = 500;
    std::vector<int> addition(20, partitionValue);
    data.insert(data.end(), addition.begin(), addition.end());
    std::random_device dev;
    std::mt19937 rng(dev());
    std::shuffle(data.begin(), data.end(), rng);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("partition", "std::partition", data);
        set.runStopwatch();
        std::partition(f1, l1, [partitionValue] (int x) { return x == partitionValue; });
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("stable_partition", "std::stable_partition", data);
        set.runStopwatch();
        std::stable_partition(f1, l1, [partitionValue] (int x) { return x == partitionValue; });
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    std::vector<int> dst_true(count);
    std::vector<int> dst_false(count);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("partition_copy_src", "std::partition_copy src", data);
        auto [f2, l2] = set.add("partition_copy_dst_true", "std::partition_copy dst_true", dst_true);
        auto [f3, l3] = set.add("partition_copy_dst_false", "std::partition_copy dst_false", dst_false);
        set.runStopwatch();
        std::partition_copy(f1, l1, f2, f3, [partitionValue] (int x) { return x == partitionValue; });
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


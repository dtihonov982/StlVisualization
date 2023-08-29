#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, reduce_accumulate) {
    std::vector<int> data = getRandVector(50, 1, 1000);
    int sum = std::accumulate(data.begin(), data.end(), 0);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("reduce", "std::reduce", data);
        set.runStopwatch();
        auto result = std::reduce(f1, l1);
        set.save();
        EXPECT_EQ(result, sum);
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("accumulate", "std::accumulate", data);
        set.runStopwatch();
        auto result = std::accumulate(f1, l1, 0);
        set.save();
        EXPECT_EQ(result, sum);
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

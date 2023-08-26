#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, transform_numeric) {
    int size = 100;
    std::vector<int> input{0, 2, 4, 6, 8};
    std::vector<int> output{1, 3, 5, 7, 9};
    int init = 1;
    auto bin_reduce = [] (int x, int y) { return x + y; };
    auto bin_transform = [] (int x, int y) { return x + y; };

    try {
        RecordingSet set;
        auto [f1, l1] = set.add("transform_reduce_input", input);
        auto [f2, l2] = set.add("transform_reduce_output", output);
        set.runStopwatch();
        int result = std::transform_reduce(f1, l1, f2, init, bin_reduce, bin_transform);
        set.save();
        EXPECT_EQ(result, 46);
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


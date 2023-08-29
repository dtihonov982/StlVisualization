#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, next_permutation) {
    int size = 4;
    int times = 4*3*2;
    std::vector<int> data(size);
    std::iota(data.rbegin(), data.rend(), 1);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("next_permutation", "std::next_permutation", data);
        set.runStopwatch();
        for (int i = 0; i < times; ++i)
            std::next_permutation(f1, l1);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, is_permutation) {
    std::vector<int> first(30);
    std::iota(first.begin(), first.end(), 1);

    auto second{first};
    std::random_device dev;
    std::mt19937 rng(dev());
    std::shuffle(second.begin(), second.end(), rng);

    try {
        RecordingSet<decltype(first)> set;
        auto [f1, l1] = set.add("is_permutation_first", first);
        auto [f2, l2] = set.add("is_permutation_second", second);
        set.runStopwatch();
        std::is_permutation(f1, l1, f2);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

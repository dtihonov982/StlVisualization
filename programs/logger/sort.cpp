#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, sort) {
    int count = 80;
    std::vector<int> data = getRandVector(count, 1, 500);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("sort", "std::sort", data);
        set.runStopwatch();
        std::sort(f1, l1);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("stable_sort", "std::stable_sort", data);
        set.runStopwatch();
        std::stable_sort(f1, l1);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("partial_sort", "std::partial_sort", data);
        set.runStopwatch();
        std::partial_sort(f1, f1 + count / 2, l1);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("nth_element", "std::nth_element", data);
        set.runStopwatch();
        std::nth_element(f1, f1 + count / 2, l1);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    std::vector<int> dst(count);
    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("partial_sort_copy_src", "std::partial_sort_copy src", data);
        auto [f2, l2] = set.add("partial_sort_copy_dst", "std::partial_sort_copy dst", dst);
        set.runStopwatch();
        std::partial_sort_copy(f1, l1, f2, f2 + count / 2);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


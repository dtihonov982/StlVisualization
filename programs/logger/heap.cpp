#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, heap) {
    std::vector<int> data = getRandVector(30, 1, 25);
    auto data_heap(data);
    std::make_heap(data_heap.begin(), data_heap.end());

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("make_heap", "std::make_heap", data);
        set.runStopwatch();
        std::make_heap(f1, l1);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("is_heap", "std::is_heap", data_heap);
        set.runStopwatch();
        auto res = std::is_heap(f1, l1);
        set.save();
        EXPECT_TRUE(res);
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("sort_heap", "std::sort_heap", data_heap);
        set.runStopwatch();
        std::sort_heap(f1, l1);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("full_sort_heap", "std::make_heap and std::sort_heap", data);
        set.runStopwatch();
        std::make_heap(f1, l1);
        std::sort_heap(f1, l1);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("default_sort", "std::sort", data);
        set.runStopwatch();
        std::sort(f1, l1);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


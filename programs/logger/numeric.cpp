#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, inner_product) {
    int count = 10;
    std::vector<int> first(count);
    std::iota(first.begin(), first.end(), 1);
    std::vector<int> second(count);
    std::iota(second.begin(), second.end(), 21);

    try {
        RecordingSet<decltype(first)> set;
        auto [f1, l1] = set.add("inner_product_src", "std::inner_product src", first);
        auto [f2, l2] = set.add("inner_product_dst", "std::inner_product dst", second);
        set.runStopwatch();
        auto res = std::inner_product(f1, l1, f2, 0);

        set.save();
        EXPECT_EQ(res, 1485);
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

}

TEST(std_algorithm, partial_sum) {
    int count = 100;
    std::vector<int> first(count);
    std::iota(first.begin(), first.end(), 1);
    std::vector<int> second(count);
    std::iota(second.begin(), second.end(), 21);
    std::vector<int> dst(count);

    try {
        RecordingSet<decltype(first)> set;
        auto [f1, l1] = set.add("partial_sum_src", "std::partial_sum src", first);
        auto [f2, l2] = set.add("partial_sum_dst", "std::partial_sum dst", dst);
        set.runStopwatch();
        std::partial_sum(f1, l1, f2);

        set.save();
        EXPECT_EQ(*(l2 - 1), count * (count + 1) / 2);
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

TEST(std_algorithm, inclusive_scan) {
    int count = 10;
    std::vector<int> first(count);
    std::iota(first.begin(), first.end(), 1);

    std::vector<int> dst(count);

    try {
        RecordingSet<decltype(first)> set;
        auto [f1, l1] = set.add("inclusive_scan_src", "std::inclusive_scan src", first);
        auto [f2, l2] = set.add("inclusive_scan_dst", "std::inclusive_scan dst", dst);
        set.runStopwatch();
        std::inclusive_scan(f1, l1, f2);

        set.save();
        EXPECT_EQ(*(l2 - 1), count * (count + 1) / 2);
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

TEST(std_algorithm, exclusive_scan) {
    int count = 10;
    std::vector<int> first(count);
    std::iota(first.begin(), first.end(), 1);
    std::vector<int> dst(count);
    int init = 0;

    try {
        RecordingSet<decltype(first)> set;
        auto [f1, l1] = set.add("exclusive_scan_src", "std::exclusive_scan src", first);
        auto [f2, l2] = set.add("exclusive_scan_dst", "std::exclusive_scan dst", dst);
        set.runStopwatch();
        std::exclusive_scan(f1, l1, f2, init);

        set.save();
        EXPECT_EQ(*(l2 - 1), (count - 1) * count / 2 + init);
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

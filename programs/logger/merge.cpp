#include <gtest/gtest.h>

#include <algorithm>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, merge) {
    std::vector<int> a = getRandVector(80, 1, 100);
    std::sort(a.begin(), a.end());
    std::vector<int> b = getRandVector(80, 1, 100);
    std::sort(b.begin(), b.end());
    std::vector<int> result(160);
    

    try {
        RecordingSet<decltype(a)> set;
        auto [f1, l1] = set.add("merge_first", "std::merge first", a);
        auto [f2, l2] = set.add("merge_second", "std::merge second", b);
        auto [f3, l3] = set.add("merge_result", "std::merge result", result);
        set.runStopwatch();
        std::merge(f1, l1, f2, l2, f3);
        set.save();
        EXPECT_TRUE(std::is_sorted(f3, l3));
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


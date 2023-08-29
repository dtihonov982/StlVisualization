#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, reverse_copy) {
    std::vector<int> src(50);
    std::iota(src.begin(), src.end(), 1);
    std::vector<int> dst(50);

    try {
        RecordingSet<decltype(src)> set;
        auto [f1, l1] = set.add("reverse_copy_src", "std::reverse_copy src", src);
        auto [f2, l2] = set.add("reverse_copy_dst", "std::reverse_copy dst", dst);
        set.runStopwatch();
        std::reverse_copy(f1, l1, f2);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


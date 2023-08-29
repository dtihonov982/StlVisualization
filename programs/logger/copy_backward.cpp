#include <gtest/gtest.h>

#include <algorithm>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, copy_backward) {
    std::vector<int> src = getRandVector(50, 1, 100);
    std::vector<int> dst(50);

    try {
        RecordingSet<decltype(src)> set;
        auto [f1, l1] = set.add("copy_backward_src", "std::copy_backward src", src);
        auto [f2, l2] = set.add("copy_backward_dst", "std::copy_backward dst", dst);
        set.runStopwatch();
        std::copy_backward(f1, l1, l2);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


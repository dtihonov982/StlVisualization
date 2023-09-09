#include <gtest/gtest.h>

#include <algorithm>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, adjacent_difference) {
    std::vector<int> data = getRandVector(50, 1, 100);
    std::vector<int> result(50);

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("adjacent_difference_src", "std::adjacent_difference src", data);
        auto [f2, l2] = set.add("adjacent_difference_dst", "std::adjacent_differenceb dst", result);
        set.runStopwatch();
        std::adjacent_difference(f1, l1, f2);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


#include <gtest/gtest.h>

#include <algorithm>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, sort) {
    std::vector<int> data = getRandVector(50, 1, 1000);

    try {
        RecordingSet set;
        auto [f1, l1] = set.add("sort_50", data);
        set.runStopwatch();
        std::sort(f1, l1);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


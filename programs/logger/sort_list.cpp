#include <gtest/gtest.h>

#include <algorithm>
#include <list>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Common/Common.h"

TEST(std_algorithm, sort) {
    std::vector<int> tmp = getRandVector(50, 1, 100);
    std::list<int> data(tmp.begin(), tmp.end());

    try {
        RecordingSet<decltype(data)> set;
        auto [f1, l1] = set.add("sort_list", "std::sort and std::list", data);
        set.runStopwatch();
        std::sort(f1, l1);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


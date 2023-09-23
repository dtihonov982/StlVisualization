#include <gtest/gtest.h>

#include "Common/Exception.h"
#include "Logger/RecordingSet.h"
#include "Logger/MyAlgorithm.h"
#include "Common/Common.h"

TEST(std_algorithm, my_copy) {
    std::vector<int> a {101, 202, 303};
    std::vector<int> b(3);

    try {
        RecordingSet<decltype(a)> set;
        auto [f1, l1] = set.add("my_copy_src", "my::copy src", a);
        auto [f2, l2] = set.add("my_copy_dst", "my::copy dst", b);
        set.runStopwatch();
        my::copy(f1, l1, f2);
        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}


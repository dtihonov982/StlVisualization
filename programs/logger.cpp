#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <fstream>
#include <random>

#include "Common/Common.h"
#include "Common/Exception.h"
#include "Logger/AccessLogger.h"
#include "Logger/NotifyingIterator.h"
#include "Logger/Recorder.h"
#include "Logger/NullLogger.h"
#include "Logger/MyAlgorithm.h"
#include "Logger/RecordingSet.h"

#if 0
std::string getPath(std::string_view algoName) {
    std::string path{"logs/"};
    path += algoName;
    path += ".txt";
    return path;
}

//return two NotifyingIterators to begin and end of the data
std::pair<NIter, NIter>
getNI(std::vector<int>& data, IEventHandler& handler) {
    auto begin = NotifyingIterator(data.begin(), data.begin(), handler);
    auto end = NotifyingIterator(data.begin(), data.end(), handler);
    return {begin, end};
}

//return three NotifyingIterators
//1. begin, 2. begin + middlePos, 3. end
std::tuple<NIter, NIter, NIter>
getNI(std::vector<int>& data, IEventHandler& handler, size_t middlePos) {
    assert(middlePos < data.size());
    auto middle = NotifyingIterator(data.begin(), data.begin() + middlePos, handler);
    auto [begin, end] = getNI(data, handler);
    return {begin, middle, end};
}

TEST(std_algorithm, sort) {
    std::vector<int> data = getRandVector(50, 1, 1000);

    try {
        Recorder sort(data, "sort");

        auto [begin, end] = sort.getIterators();
        std::sort(begin, end);

        sort.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what() << "\n";
    }
}

TEST(std_algorithm, nth_element) {
    std::vector<int> data(10);
    std::iota(data.rbegin(), data.rend(), 0);

    std::ofstream file{getPath("nth_element")};
    file << "nth_element\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto mid_ = NotifyingIterator(data.begin(), data.begin() + data.size() / 2, logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::nth_element(begin_, mid_, end_);

    logger.finalize();
}

TEST(std_algorithm, partial_sort) {
    std::vector<int> data = getRandVector(30, 0, 100);

    std::ofstream file{getPath("partial_sort")};
    file << "partial_sort\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto mid_ = NotifyingIterator(data.begin(), data.begin() + data.size() / 2, logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::partial_sort(begin_, mid_, end_);

    logger.finalize();
}

TEST(std_algorithm, partial_sum) {
    std::vector<int> data = getRandVector(50, 0, 100);
    std::ofstream file{getPath("partial_sum")};
    file << "partial_sum\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::partial_sum(begin_, end_, begin_);

    logger.finalize();
}


TEST(std_algorithm, rotate) {
    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 0);

    std::ofstream file{getPath("rotate")};
    file << "rotate\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto mid_ = NotifyingIterator(data.begin(), data.begin() + 50, logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::rotate(begin_, mid_, end_);

    logger.finalize();
}

TEST(std_algorithm, transform_rand) {
    std::vector<int> data = getRandVector(50, 0, 100);
    std::ofstream file{getPath("transform rand")};
    file << "transform rand *2\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::transform(begin_, end_, begin_, [] (int x) { return 2 *x; });

    logger.finalize();
}

TEST(std_algorithm, transform) {
    std::vector<int> data(10, 1);
    std::ofstream file{getPath("transform")};
    file << "transform *2\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::transform(begin_, end_, begin_, [] (int x) { return 2 *x; });

    logger.finalize();
}

//not representative
TEST(std_algorithm, unique) {
    
    std::vector<int> data = getRandVector(50, 0, 4);
    std::ofstream file{getPath("unique")};
    file << "unique\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::unique(begin_, end_);

    logger.finalize();
}

TEST(std_algorithm, reverse) {
    std::vector<int> data(50);
    std::iota(data.begin(), data.end(), 1);

    std::ofstream file{getPath("reverse")};
    file << "reverse\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::reverse(begin_, end_);

    logger.finalize();
}

TEST(std_algorithm, search) {
    std::vector<int> data(50);
    std::iota(data.begin(), data.end(), 1);

    std::vector<int> needle(data.begin() + 10, data.begin() + 20);

    std::ofstream file{getPath("search")};
    file << "search\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin = NotifyingIterator(data.begin(), data.begin(), logger);
    auto end = NotifyingIterator(data.begin(), data.end(), logger);

    NullLogger nullLogger{};
    auto begin2 = NotifyingIterator(needle.begin(), needle.begin(), nullLogger);
    auto end2 = NotifyingIterator(needle.begin(), needle.end(), nullLogger);

    std::search(begin, end, begin2, end2);

    logger.finalize();
}

TEST(std_algorithm, inplace_merge) {
    std::vector<int> data = getRandVector(50, 1, 300);
    std::sort(data.begin(), data.begin() + data.size() / 2);
    std::sort(data.begin() + data.size() / 2, data.end());

    std::ofstream file{getPath("inplace_merge")};
    file << "inplace_merge\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, mid, end] = getNI(data, logger, data.size() / 2);

    std::inplace_merge(begin, mid, end);

    logger.finalize();
}

TEST(std_algorithm, up_low_bound) {
    std::vector<int> data = getRandVector(30, 1, 300);
    std::vector<int> equal(20, 150);
    data.insert(data.end(), equal.begin(), equal.end());

    std::sort(data.begin(), data.end());

    //lower bound
    {
    std::ofstream file{getPath("lower_bound")};
    file << "lower bound\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);

    auto it = std::lower_bound(begin, end, 150);
    EXPECT_EQ(*it, 150);
    }

    //upper_bound
    {
    std::ofstream file{getPath("upper_bound")};
    file << "upper bound\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);

    auto it = std::upper_bound(begin, end, 150);
    EXPECT_GT(*it, 150);

    logger.finalize();
    }
    
    //equal_range
    {
    std::ofstream file{getPath("equal_range")};
    file << "equal range\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);

    auto [first, last] = std::equal_range(begin, end, 150);
    *first; *last;
    logger.finalize();

    EXPECT_TRUE(std::all_of(first, last, [] (int x) { return x == 150; }));

    }
}

TEST(std_algorithm, reduce_accumulate) {
    std::vector<int> data = getRandVector(50, 1, 1000);
    int sum = std::accumulate(data.begin(), data.end(), 0);

    {
    std::ofstream file{getPath("reduce")};
    file << "reduce\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);
    int res = std::reduce(begin, end);
    EXPECT_EQ(res, sum);

    logger.finalize();
    }
    
    {
    std::ofstream file{getPath("accumulate")};
    file << "accumulate\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);
    int res = std::accumulate(begin, end, 0);
    EXPECT_EQ(res, sum);

    logger.finalize();
    }
}




TEST(std_algorithm, scan) {
    std::vector<int> data = getRandVector(10, 1, 25);
    //int sum = std::accumulate(data.begin(), data.end(), 0);

    try {
        auto startPoint = std::chrono::high_resolution_clock::now();
        //input data
        Recorder srcRecorder(data, "inclusive_scan", startPoint);
        auto [begin, end] = srcRecorder.getIterators();

        //output data
        std::vector<int> result(10);
        Recorder dstRecorder(result, "inclusive_scan_result", startPoint);

        auto [beginDst, endDst] = dstRecorder.getIterators();

        //algorithm
        std::inclusive_scan(begin, end, beginDst);

        srcRecorder.finalize();
        dstRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Recorder srcRecorder(data, "partial_sum");
        auto [begin, end] = srcRecorder.getIterators();

        //output data
        std::vector<int> result(10);
        Recorder dstRecorder(result, "partial_sum_result");

        auto [beginDst, endDst] = dstRecorder.getIterators();

        //algorithm
        std::partial_sum(begin, end, beginDst);

        srcRecorder.finalize();
        dstRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        //input data
        Recorder srcRecorder(data, "exclusive_scan");
        auto [begin, end] = srcRecorder.getIterators();

        //output data
        std::vector<int> result(10);
        Recorder dstRecorder(result, "exclusive_scan_result");

        auto [beginDst, endDst] = dstRecorder.getIterators();

        //algorithm
        std::exclusive_scan(begin, end, beginDst, 0);

        srcRecorder.finalize();
        dstRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
}

TEST(std_algorithm, heap) {
    std::vector<int> data = getRandVector(30, 1, 25);

    try {
        Recorder recorder(data, "make_heap");
        auto [begin, end] = recorder.getIterators();
        std::make_heap(begin, end);
        recorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    std::make_heap(data.begin(), data.end());
    
    try {
        Recorder recorder(data, "is_heap");
        auto [begin, end] = recorder.getIterators();
        std::is_heap(begin, end);
        recorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        Recorder recorder(data, "push_heap");
        auto [begin, end] = recorder.getIterators();
        std::push_heap(begin, end);
        recorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        Recorder recorder(data, "pop_heap");
        auto [begin, end] = recorder.getIterators();
        std::pop_heap(begin, end);
        recorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        Recorder recorder(data, "sort_heap");
        auto [begin, end] = recorder.getIterators();
        std::sort_heap(begin, end);
        recorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

}

TEST(std_algorithm, set) {
    std::vector<int> A = getRandVector(20, 100, 199);
    std::sort(A.begin(), A.end());
    std::vector<int> C = getRandVector(10, 200, 299);
    std::sort(C.begin(), C.end());
    A.insert(A.end(), C.begin(), C.end()); //A += C
    ASSERT_TRUE(std::is_sorted(A.begin(), A.end()));
    std::vector<int> B = getRandVector(20, 300, 499);
    std::sort(B.begin(), B.end());
    B.insert(B.begin(), C.begin(), C.end()); //B += C
    ASSERT_TRUE(std::is_sorted(B.begin(), B.end()));
    std::vector<int> R(60);

    try {
        //input data
        Recorder ARecorder(A, "includes_A");
        auto [beginA, endA] = ARecorder.getIterators();

        Recorder CRecorder(C, "includes_C");
        auto [beginC, endC] = CRecorder.getIterators();

        //algorithm
        bool result = std::includes(beginA, endA, beginC, endC);
        ASSERT_TRUE(result);

        ARecorder.finalize();
        CRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Recorder ARecorder(A, "set_differrence_A");
        auto [beginA, endA] = ARecorder.getIterators();

        Recorder BRecorder(B, "set_differrence_B");
        auto [beginB, endB] = BRecorder.getIterators();

        Recorder RRecorder(R, "set_differrence_R");
        auto [beginR, endR] = RRecorder.getIterators();

        //algorithm
        std::set_difference(beginA, endA, beginB, endB, beginR);
        ARecorder.finalize();
        BRecorder.finalize();
        RRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Recorder ARecorder(A, "set_intersection_A");
        auto [beginA, endA] = ARecorder.getIterators();

        Recorder BRecorder(B, "set_intersection_B");
        auto [beginB, endB] = BRecorder.getIterators();

        Recorder RRecorder(R, "set_intersection_R");
        auto [beginR, endR] = RRecorder.getIterators();

        //algorithm
        std::set_intersection(beginA, endA, beginB, endB, beginR);
        ARecorder.finalize();
        BRecorder.finalize();
        RRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Recorder ARecorder(A, "set_symmetric_difference_A");
        auto [beginA, endA] = ARecorder.getIterators();

        Recorder BRecorder(B, "set_symmetric_difference_B");
        auto [beginB, endB] = BRecorder.getIterators();

        Recorder RRecorder(R, "set_symmetric_difference_R");
        auto [beginR, endR] = RRecorder.getIterators();

        //algorithm
        std::set_symmetric_difference(beginA, endA, beginB, endB, beginR);
        ARecorder.finalize();
        BRecorder.finalize();
        RRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Recorder ARecorder(A, "set_union_A");
        auto [beginA, endA] = ARecorder.getIterators();

        Recorder BRecorder(B, "set_union_B");
        auto [beginB, endB] = BRecorder.getIterators();

        Recorder RRecorder(R, "set_union_R");
        auto [beginR, endR] = RRecorder.getIterators();

        //algorithm
        std::set_union(beginA, endA, beginB, endB, beginR);
        ARecorder.finalize();
        BRecorder.finalize();
        RRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
}

TEST(my_algorithm, copy) {
    std::vector<int> src(3, 101);
    std::vector<int> dst(3);
    try {
        auto startPoint = std::chrono::high_resolution_clock::now();
        //input data
        Recorder srcRecorder(src, "my_copy_src", startPoint);
        auto [beginSrc, endSrc] = srcRecorder.getIterators();

        Recorder dstRecorder(dst, "my_copy_dst", startPoint);
        auto [beginDst, endDst] = dstRecorder.getIterators();

        //algorithm
        my::copy(beginSrc, endSrc, beginDst);
        srcRecorder.finalize();
        dstRecorder.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

}
#endif

TEST(std_algorithm, small_sort) {
    std::vector<int> data(10);
    std::iota(data.rbegin(), data.rend(), 1);

    try {
        //input data
        Recorder recorder("small_sort", data);
        auto [first, last] = recorder.getIterators();

        //algorithm
        std::sort(first, last);
        recorder.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

}

TEST(my_algorithm, double_each) {
    std::vector<int> src(3, 101);
    try {
        //input data
        Recorder recorder("double_each", src);
        auto [first, last] = recorder.getIterators();

        //algorithm
        my::double_each(first, last);
        recorder.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

TEST(std_algorithm, scan) {
    std::vector<int> src = getRandVector(10, 1, 25);
    std::vector<int> dst(10);

    try {
        RecordingSet set;
        auto [f1, l1] = set.add("inclusive_scan_src", src);
        auto [f2, l2] = set.add("inclusive_scan_dst", dst);

        set.startStopwatch();
        std::inclusive_scan(f1, l1, f2);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

TEST(my_algorithm, copy_first) {
    std::vector<int> src {101, 202, 303};
    std::vector<int> dst(3, 0);

    try {
        RecordingSet set;
        auto [f1, l1] = set.add("copy_first_src", src);
        auto [f2, l2] = set.add("copy_first_dst", dst);

        set.startStopwatch();
        std::inclusive_scan(f1, l1, f2);

        set.save();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}

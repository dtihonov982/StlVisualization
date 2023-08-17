#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <fstream>
#include <random>

#include "AccessLogger.h"
#include "NotifyingIterator.h"
#include "Common.h"
#include "Exception.h"
#include "Case.h"
#include "NullLogger.h"
#include "MyAlgorithm.h"

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
        Case sort(data, "sort");

        auto [begin, end] = sort.getIterators();
        std::sort(begin, end);

        sort.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what() << "\n";
    }
}

TEST(std_algorithm, small_sort) {
    std::vector<int> data(10);
    std::iota(data.rbegin(), data.rend(), 0);

    std::ofstream file{getPath("small_sort")};
    file << "small sort\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), data.begin(), logger);
    auto end_ = NotifyingIterator(data.begin(), data.end(), logger);

    std::sort(begin_, end_);

    logger.finalize();
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
        Case srcCase(data, "inclusive_scan", startPoint);
        auto [begin, end] = srcCase.getIterators();

        //output data
        std::vector<int> result(10);
        Case dstCase(result, "inclusive_scan_result", startPoint);

        auto [beginDst, endDst] = dstCase.getIterators();

        //algorithm
        std::inclusive_scan(begin, end, beginDst);

        srcCase.finalize();
        dstCase.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Case srcCase(data, "partial_sum");
        auto [begin, end] = srcCase.getIterators();

        //output data
        std::vector<int> result(10);
        Case dstCase(result, "partial_sum_result");

        auto [beginDst, endDst] = dstCase.getIterators();

        //algorithm
        std::partial_sum(begin, end, beginDst);

        srcCase.finalize();
        dstCase.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        //input data
        Case srcCase(data, "exclusive_scan");
        auto [begin, end] = srcCase.getIterators();

        //output data
        std::vector<int> result(10);
        Case dstCase(result, "exclusive_scan_result");

        auto [beginDst, endDst] = dstCase.getIterators();

        //algorithm
        std::exclusive_scan(begin, end, beginDst, 0);

        srcCase.finalize();
        dstCase.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
}

TEST(std_algorithm, heap) {
    std::vector<int> data = getRandVector(30, 1, 25);

    try {
        Case case_(data, "make_heap");
        auto [begin, end] = case_.getIterators();
        std::make_heap(begin, end);
        case_.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    std::make_heap(data.begin(), data.end());
    
    try {
        Case case_(data, "is_heap");
        auto [begin, end] = case_.getIterators();
        std::is_heap(begin, end);
        case_.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        Case case_(data, "push_heap");
        auto [begin, end] = case_.getIterators();
        std::push_heap(begin, end);
        case_.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        Case case_(data, "pop_heap");
        auto [begin, end] = case_.getIterators();
        std::pop_heap(begin, end);
        case_.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

    try {
        Case case_(data, "sort_heap");
        auto [begin, end] = case_.getIterators();
        std::sort_heap(begin, end);
        case_.finalize();
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
        Case ACase(A, "includes_A");
        auto [beginA, endA] = ACase.getIterators();

        Case CCase(C, "includes_C");
        auto [beginC, endC] = CCase.getIterators();

        //algorithm
        bool result = std::includes(beginA, endA, beginC, endC);
        ASSERT_TRUE(result);

        ACase.finalize();
        CCase.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Case ACase(A, "set_differrence_A");
        auto [beginA, endA] = ACase.getIterators();

        Case BCase(B, "set_differrence_B");
        auto [beginB, endB] = BCase.getIterators();

        Case RCase(R, "set_differrence_R");
        auto [beginR, endR] = RCase.getIterators();

        //algorithm
        std::set_difference(beginA, endA, beginB, endB, beginR);
        ACase.finalize();
        BCase.finalize();
        RCase.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Case ACase(A, "set_intersection_A");
        auto [beginA, endA] = ACase.getIterators();

        Case BCase(B, "set_intersection_B");
        auto [beginB, endB] = BCase.getIterators();

        Case RCase(R, "set_intersection_R");
        auto [beginR, endR] = RCase.getIterators();

        //algorithm
        std::set_intersection(beginA, endA, beginB, endB, beginR);
        ACase.finalize();
        BCase.finalize();
        RCase.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Case ACase(A, "set_symmetric_difference_A");
        auto [beginA, endA] = ACase.getIterators();

        Case BCase(B, "set_symmetric_difference_B");
        auto [beginB, endB] = BCase.getIterators();

        Case RCase(R, "set_symmetric_difference_R");
        auto [beginR, endR] = RCase.getIterators();

        //algorithm
        std::set_symmetric_difference(beginA, endA, beginB, endB, beginR);
        ACase.finalize();
        BCase.finalize();
        RCase.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }
    
    try {
        //input data
        Case ACase(A, "set_union_A");
        auto [beginA, endA] = ACase.getIterators();

        Case BCase(B, "set_union_B");
        auto [beginB, endB] = BCase.getIterators();

        Case RCase(R, "set_union_R");
        auto [beginR, endR] = RCase.getIterators();

        //algorithm
        std::set_union(beginA, endA, beginB, endB, beginR);
        ACase.finalize();
        BCase.finalize();
        RCase.finalize();
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
        Case srcCase(src, "my_copy_src", startPoint);
        auto [beginSrc, endSrc] = srcCase.getIterators();

        Case dstCase(dst, "my_copy_dst", startPoint);
        auto [beginDst, endDst] = dstCase.getIterators();

        //algorithm
        my::copy(beginSrc, endSrc, beginDst);
        srcCase.finalize();
        dstCase.finalize();
    }
    catch (const Exception& ex) {
        FAIL() << ex.what();
    }

}


TEST(my_algorithm, double_each) {
    std::vector<int> src(3, 101);
    try {
        auto startPoint = std::chrono::high_resolution_clock::now();
        //input data
        Case case_(src, "double_each", startPoint);
        auto [first, last] = case_.getIterators();

        //algorithm
        my::double_each(first, last);
        case_.finalize();
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

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

class NullLogger: public IEventHandler {
public:
    void handle(Event& e) override {}
};

using OriginalIterator = std::vector<int>::iterator;
using NIter = NotifyingIterator<OriginalIterator>;

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

std::string getPath(std::string_view algoName) {
    std::string path{"logs/"};
    path += algoName;
    path += ".txt";
    return path;
}

TEST(std_algorithm, sort) {
    std::vector<int> data = getRandVector(50, 1, 1000);

    std::ofstream file{getPath("sort")};
    file << "sort\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);
    std::sort(begin, end);

    logger.finalize();
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
    std::vector<int> data = getRandVector(10, 1, 1000);
    int sum = std::accumulate(data.begin(), data.end(), 0);

    {
    //input data
    std::ofstream file{getPath("inclusive_scan")};
    file << "inclusive scan\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);

    //output data
    std::vector<int> result(10);
    std::ofstream file_result{getPath("inclusive_scan_result")};
    file_result << "inclusive scan result\n" << result << '\n';

    AccessLogger logger_result{result, file_result};

    auto [begin_result, end_result] = getNI(result, logger_result);

    //algorithm
    std::inclusive_scan(begin, end, begin_result);

    logger.finalize();
    logger_result.finalize();
    }
    
    {
    std::ofstream file{getPath("partial_sum")};
    file << "partial sum\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);
    std::partial_sum(begin, end, begin);

    logger.finalize();
    }

    {
    std::ofstream file{getPath("exclusive_scan")};
    file << "exclusive scan\n" << data << '\n';

    AccessLogger logger{data, file};

    auto [begin, end] = getNI(data, logger);
    std::exclusive_scan(begin, end, begin, 0);

    logger.finalize();
    }
    
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    //std::random_device rd;
    //std::mt19937 g(rd());
    //std::vector<int> data {10, 20};
    //std::vector<int> data(100);
    //std::iota(data.rbegin(), data.rend(), 0);
    /*
    for (int i = 0; i < 50; ++i) data.push_back(500);
    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 1);
    
    */


    //std::sort(begin_, end_);
    //std::rotate(begin_, middle_, end_);
    //std::make_heap(begin_, middle_);
    //std::sort_heap(begin_, middle_);
    //std::find(begin_, end_, 10);
    //std::partition(begin_, end_, [] (int x) { return x == 500; });
    //std::shuffle(begin_, end_, g);
    //int acc = std::accumulate(begin_, end_, 0);
    //assert(acc == 45);
    //std::iter_swap(begin_, begin_ + 1);
    //std::lower_bound(begin_, end_, 10);
    //assert(std::is_sorted(begin_, end_));
    //assert(*std::max_element(begin_, end_) == 9);
    //std::binary_search(begin_, end_, 1);


    return 0;
}

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

std::string getPath(std::string_view algoName) {
    std::string path{"logs/"};
    path += algoName;
    path += ".txt";
    return path;
}

void sort() {
    std::vector<int> data(10);
    std::iota(data.rbegin(), data.rend(), 0);

    std::ofstream file{getPath("sort")};
    file << "small sort\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), logger);

    std::sort(begin_, end_);

    logger.finalize();
}

void nth_element() {
    std::vector<int> data(10);
    std::iota(data.rbegin(), data.rend(), 0);

    std::ofstream file{getPath("nth_element")};
    file << "nth_element\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto mid_ = NotifyingIterator(data.begin() + data.size() / 2, data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), data.begin(), logger);

    std::nth_element(begin_, mid_, end_);

    logger.finalize();
}

void partial_sort() {
    std::vector<int> data = getRandVector(30, 0, 100);

    std::ofstream file{getPath("partial_sort")};
    file << "partial_sort\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto mid_ = NotifyingIterator(data.begin() + data.size() / 2, data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), logger);

    std::partial_sort(begin_, mid_, end_);

    logger.finalize();
}

void partial_sum() {
    std::vector<int> data = getRandVector(50, 0, 100);
    std::ofstream file{getPath("partial_sum")};
    file << "partial_sum\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), logger);

    std::partial_sum(begin_, end_, begin_);

    logger.finalize();
}


void rotate() {
    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 0);

    std::ofstream file{getPath("rotate")};
    file << "rotate\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto mid_ = NotifyingIterator(data.begin() + 50, logger);
    auto end_ = NotifyingIterator(data.end(), logger);

    std::rotate(begin_, mid_, end_);

    logger.finalize();
}

void transform_rand() {
    std::vector<int> data = getRandVector(50, 0, 100);
    std::ofstream file{getPath("transform rand")};
    file << "transform rand *2\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), logger);

    std::transform(begin_, end_, begin_, [] (int x) { return 2 *x; });

    logger.finalize();
}

void transform() {
    std::vector<int> data(10, 1);
    std::ofstream file{getPath("transform")};
    file << "transform *2\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), logger);

    std::transform(begin_, end_, begin_, [] (int x) { return 2 *x; });

    logger.finalize();
}

//not representative
void unique() {
    
    std::vector<int> data = getRandVector(50, 0, 4);
    std::ofstream file{getPath("unique")};
    file << "unique\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), logger);

    std::unique(begin_, end_);

    logger.finalize();
}

void reverse() {
    std::vector<int> data(50);
    std::iota(data.begin(), data.end(), 1);

    std::ofstream file{getPath("reverse")};
    file << "reverse\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto end_ = NotifyingIterator(data.end(), data.begin(), logger);

    std::reverse(begin_, end_);

    logger.finalize();
}

void search() {
    std::vector<int> data(50);
    std::iota(data.begin(), data.end(), 1);

    std::vector<int> needle(data.begin() + 10, data.begin() + 20);

    std::ofstream file{getPath("search")};
    file << "search\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin = NotifyingIterator(data.begin(), logger);
    auto end = NotifyingIterator(data.end(), data.begin(), logger);

    NullLogger nullLogger{};
    auto begin2 = NotifyingIterator(needle.begin(), nullLogger);
    auto end2 = NotifyingIterator(needle.end(), needle.begin(), nullLogger);

    std::search(begin, end, begin2, end2);

    logger.finalize();
}

void inplace_merge() {
    std::vector<int> data = getRandVector(50, 1, 300);
    std::sort(data.begin(), data.begin() + data.size() / 2);
    std::sort(data.begin() + data.size() / 2, data.end());

    std::ofstream file{getPath("inplace_merge")};
    file << "inplace_merge\n" << data << '\n';

    AccessLogger logger{data, file};

    auto begin = NotifyingIterator(data.begin(), logger);
    auto mid = NotifyingIterator(data.begin() + data.size() / 2, logger);
    auto end = NotifyingIterator(data.end(), logger);

    std::inplace_merge(begin, mid, end);

    logger.finalize();
}

int main() {
    sort();
    nth_element();
    partial_sort();
    partial_sum();
    rotate();
    transform();
    unique();
    reverse();
    search();
    inplace_merge();

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

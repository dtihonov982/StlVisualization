#include <algorithm>
#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <fstream>
#include <random>

#include "Logger.h"





int main() {
    std::random_device rd;
    std::mt19937 g(rd());
    //std::vector<int> data {10, 20};
    //std::vector<int> data(100);
    //std::iota(data.rbegin(), data.rend(), 0);
    /*
    for (int i = 0; i < 50; ++i) data.push_back(500);
    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 1);
    
    */

    std::vector<int> data = getRandVector(50, 0, 100);
    std::ofstream file{"partial_sum.txt"};
    file << "title\n" << data << '\n';

    AccessLogger logger{data, file};
    auto begin_ = IterDecorator(data.begin(), logger);
    auto middle_ = IterDecorator(data.begin() + 50, logger);
    auto end_ = IterDecorator(data.end(), logger);

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
    std::partial_sum(begin_, end_, begin_);

    logger.finalize();


    return 0;
}

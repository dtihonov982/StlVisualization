#include <vector>
#include <algorithm>
#include <numeric>

#include "Event.h"
#include "NotifyingIterator.h"
#include "DebugLogger.h"

std::vector<int> getMonotonic(int count, int first = 1, bool ascending = true) {
    std::vector<int> data(count);
    if (ascending)
        std::iota(data.begin(), data.end(), first);
    else
        std::iota(data.rbegin(), data.rend(), first);
    return data;
}

void sortCase(IEventHandler& handler) {
    auto data = getMonotonic(10, 1, false);
    auto begin_ = NotifyingIterator(data.begin(), data.begin(), handler);
    auto end_ = NotifyingIterator(data.begin(), data.end(), handler);
    std::sort(begin_, end_);
}


int main() {
    DebugLogger log;
    sortCase(log);
    #if 0
    std::vector<float> data(10);
    std::iota(data.rbegin(), data.rend(), 0.1f);
    std::cout << data << "\n\n";
    auto begin_ = NotifyingIterator(data.begin(), log);
    auto end_ = NotifyingIterator(data.end(), log);
    std::sort(begin_, end_);

    std::cout << '\n' << data << '\n';


    //std::vector<int> data {10, 20};
    std::vector<int> data(10, 10);
    std::iota(data.begin() + 5, data.end(), 0);
    std::cout << data << "\n\n";
    AccessLogger logger{data, std::cout};
    auto begin_ = NotifyingIterator(data.begin(), logger);
    auto middle_ = NotifyingIterator(data.begin() + 1, logger);
    auto end_ = NotifyingIterator(data.end(), logger);
    //std::sort(begin_, end_);
    //std::rotate(begin_, middle_, end_);
    //std::find(begin_, end_, 10);
    //std::partition(begin_, end_, [] (int x) { return x != 10; });
    //std::unique(begin_, end_);
    //std::iter_swap(begin_, begin_ + 1);
    //std::lower_bound(begin_, end_, 10);

    bool coherence = logger.finalize();

    std::cout << '\n' << std::boolalpha << coherence;

    std::cout << '\n' << data << '\n';
    #endif

    return 0;
}

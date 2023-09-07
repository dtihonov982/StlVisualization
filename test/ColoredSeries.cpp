#include "Visual/ColoredSeries.h"

#include <algorithm>
#include <vector>

int main() {
    ColoredSeries series;
    series.setGeometry({0, 0, 100, 100});
    std::vector<int> data {1, 2, 3};
    series.load(data.cbegin(), data.cend());
    return 0;
}

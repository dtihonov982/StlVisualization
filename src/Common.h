#ifndef COMMON_H
#define COMMON_H

#include <random>
#include <iostream>
#include <vector>

std::vector<int> getRandVector(int size, int min, int max);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    if (vec.empty()) 
        return os;

    os << vec[0];
    
    for (auto it = vec.cbegin() + 1; it != vec.cend(); ++it) {
        os << "," << *it;
    }
    return os;
}

#endif // COMMON_H

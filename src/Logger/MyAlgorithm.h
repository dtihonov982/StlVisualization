#ifndef MYALGORITHM_H
#define MYALGORITHM_H

//Simple algorithms for debug recording

namespace my {

template<typename It>
It copy(It begin_src, It end_src, It begin_dst) {
    for (; begin_src < end_src; ++begin_src) {
        *begin_dst = *begin_src;
        ++begin_dst;
    }
    return begin_dst;
}

template<typename It>
void double_each(It first, It last) {
    for (; first < last; ++first) {
        *first *= 2; 
    }
}

} //my


#endif //MYALGORITHM_H

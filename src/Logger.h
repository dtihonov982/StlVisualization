#ifndef LOGGER_H
#define LOGGER_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

//AccessLogger prints messages into stream about changing containers elemetents.
//Message format: {read|write}, position[, new_value]
class AccessLogger {
public:
    using Container = std::vector<int>;
    using Iterator = typename Container::iterator;

    AccessLogger(Container& original, std::ostream& os)
    : log_(os)
    , copy_(original) 
    , original_(original)
    , begin_(original.begin()) {}
    
    
    void logAccess(const Iterator& it) {
        update();
        session_++;
        auto accessPosition = it - begin_;
        log_ << "r," << accessPosition << '\n';
    }

    void finalize() {
        update();
    }

private:
    //Compare copy and original
    //Print difference and update copy
    void update() {
        for (int i = 0; i < copy_.size(); ++i) {
            if (copy_[i] != original_[i]) {
                log_ << "w," << i << "," << original_[i] << '\n';
                copy_[i] = original_[i];
            }
        }
    }
    std::ostream& log_;

    Container copy_; //make copy of original container for tracking changes
    Container& original_; //reference to original container for comparing with original values during control
    Iterator begin_; //begin iterator for evaluating of a position of an access

    int lastAccessPos_ = -1; 
    size_t session_ = 0;
};


//Calls loggers methods if was access to original iterator
class IterDecorator {
public:
    using Container = std::vector<int>;
    using It = typename Container::iterator;

    using iterator_category = std::random_access_iterator_tag;
    using value_type = ptrdiff_t;
    using difference_type = ptrdiff_t;
    using pointer = ptrdiff_t const *;
    using reference = ptrdiff_t const &;

    IterDecorator(It origin, AccessLogger& logger): origin_(origin), logger_(logger) {}

    IterDecorator& operator=(const IterDecorator& other) {
        if (*this == other)
            return *this;
        origin_ = other.origin_;
        return *this;
    }

    IterDecorator & operator += (ptrdiff_t offset) {
        origin_ += offset;
        return *this;
    }

    IterDecorator & operator -= (ptrdiff_t offset) {
        origin_ -= offset;
        return *this;
    }

    IterDecorator operator + (ptrdiff_t offset) const {
        auto it = *this;
        it += offset;
        return it;
    }

    int const & operator [] (ptrdiff_t offset) const {
        auto it = *this;
        it += offset;
        return *it;
    }

    int & operator [] (ptrdiff_t offset) {
        auto it = *this;
        it += offset;
        return *it;
    }

    ptrdiff_t operator - (IterDecorator const & other) const {
        return origin_ - other.origin_;
    }

    IterDecorator operator - (ptrdiff_t offset) const {
        auto it = *this;
        it -= offset;
        return it;
    }

    IterDecorator & operator ++ () {
        *this += 1;
        return *this;
    }

    IterDecorator operator ++ (int) {
        IterDecorator copy(*this);
        *this += 1;
        return copy;
    }

    IterDecorator & operator -- () {
        *this -= 1;
        return *this;
    }

    int const & operator * () const {
        logger_.logAccess(origin_);
        return *origin_;
    }

    int & operator * () {
        logger_.logAccess(origin_);
        return *origin_;
    }

    bool operator == (IterDecorator const & other) const {
        return origin_ == other.origin_;
    }
    bool operator != (IterDecorator const & other) const {
        return !(*this == other);
    }

    bool operator < (IterDecorator rhs) {
        return origin_ < rhs.origin_;
    }

private:    
    AccessLogger& logger_;
    It origin_;
};


#endif // LOGGER_H

#ifndef ACCESSLOGGER_H 
#define ACCESSLOGGER_H

#include <vector>
#include <iostream>

template <typename Container>
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
        handleLastAccess();
        lastAccessPos_ = it - begin_;
    }

    bool finalize() {
        handleLastAccess();
        checkWriting();
        return copy_ == original_;
    }

    void checkWriting() {
        for (int i = 0; i < copy_.size(); ++i) {
            if (copy_[i] != original_[i]) {
                log_ << "write," << i << "," << original_[i] << '\n';
                copy_[i] = original_[i];
            }
        }
    }

private:
    void handleLastAccess() {
        if (lastAccessPos_ < 0)
            return;
        log_ << "read," << lastAccessPos_ << '\n';
        checkWriting();
    }

    std::ostream& log_;

    Container copy_;
    Container& original_;
    Iterator begin_;

    int lastAccessPos_ = -1;    
};

#endif //ACCESSLOGGER_H


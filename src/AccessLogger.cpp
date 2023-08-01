#include "AccessLogger.h"

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

void handleLastAccess() {
    if (lastAccessPos_ < 0)
        return;
    log_ << "read," << lastAccessPos_ << '\n';
    checkWriting();
}

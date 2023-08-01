#ifndef NOTIFYINGITERATOR_H
#define NOTIFYINGITERATOR_H

#include <iterator>
#include <string>

#include "Event.h"

template <typename OriginalIterator>
class NotifyingIterator {
public:
    //TODO: get from original
    using iterator_category = typename OriginalIterator::iterator_category;
    using value_type = typename OriginalIterator::value_type;
    using difference_type = typename OriginalIterator::difference_type;
    using pointer = typename OriginalIterator::pointer;
    using reference = typename OriginalIterator::reference;

    NotifyingIterator(OriginalIterator origin, IEventHandler& handler)
    : handler_(handler) 
    , original_(origin)
    , current_(origin) {}

//access operations -----------------------------------

    const reference operator [] (ptrdiff_t offset) const {
        sendMessage("const reference operator [] (ptrdiff_t offset) const");
        auto it = *this;
        it += offset;
        return *it;
    }

    reference operator [] (ptrdiff_t offset) {
        sendMessage("reference operator [] (ptrdiff_t offset)");
        auto it = *this;
        it += offset;
        return *it;
    }

    const reference operator * () const {
        sendMessage("const reference operator * () const");
        return *current_;
    }

    reference operator * () {
        sendMessage("reference operator * ()");
        return *current_;
    }

//not acccess operations ------------------------------

    NotifyingIterator& operator=(const NotifyingIterator& other) {
        sendMessage("NotifyingIterator& operator=(const NotifyingIterator& other)");
        if (*this == other)
            return *this;
        current_ = other.current_;
        original_ = other.original_;
        return *this;
    }

    NotifyingIterator & operator += (difference_type offset) {
        sendMessage("NotifyingIterator & operator += (ptrdiff_t offset)");
        current_ += offset;
        return *this;
    }

    NotifyingIterator & operator -= (difference_type offset) {
        current_ -= offset;
        return *this;
    }

    NotifyingIterator operator + (difference_type offset) const {
        auto it = *this;
        it += offset;
        return it;
    }

    bool operator == (NotifyingIterator const & other) const {
        sendMessage("operator == (NotifyingIterator const & other) const");
        return current_ == other.current_;
    }
    bool operator != (NotifyingIterator const & other) const {
        sendMessage("operator != (NotifyingIterator const & other)");
        return !(*this == other);
    }

    bool operator < (const NotifyingIterator& rhs) const {
        sendMessage("bool operator < (const NotifyingIterator& rhs) const");
        return current_ < rhs.current_;
    }

    difference_type operator - (NotifyingIterator const & other) const {
        sendMessage("difference_type operator - (NotifyingIterator const & other) const");
        return current_ - other.current_;
    }

    NotifyingIterator operator - (difference_type offset) const {
        auto it = *this;
        it -= offset;
        return it;
    }

    NotifyingIterator & operator ++ () {
        sendMessage("NotifyingIterator & operator ++ ()");
        *this += 1;
        return *this;
    }

    NotifyingIterator & operator -- () {
        *this -= 1;
        return *this;
    }

//helpers --------------------------------------

    void sendMessage(std::string text) const {
        Message msg {text};
        handler_.handle(msg);       
    }

    difference_type getOffset() {
        return std::distance(original_, current_);
    }

private:    
    IEventHandler& handler_;
    OriginalIterator original_;
    OriginalIterator current_;
};


#endif //NOTIFYINGITERATOR_H

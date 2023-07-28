#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <numeric>
#include <vector>
#include <unordered_set>

class Event {
public:
    enum Type { Message };  
    Type getType() { return type_; }  
    virtual ~Event() {}
protected:
    Event(Type type): type_(type) {}
private:
    Type type_;
};

class IEventHandler {
public:
    virtual void handle(Event& event) = 0;
    virtual ~IEventHandler() {}
};

class Message: public Event {
public:
    Message(std::string text)
    : Event(Event::Message)
    , text_(text) {}
    std::string getText() { return text_; }
private:
    std::string text_;
};

class DebugLogger: public IEventHandler {
public:
    void handle(Event& event) override {
        switch (event.getType()) {
        case Event::Message: 
            Message& msg = static_cast<Message&>(event);
            auto text = msg.getText();
            #if 0
            std::cout << text << '\n';
            #else
            auto it = unique_.find(text);
            if (it == unique_.end()) {
                unique_.insert(text);
                std::cout << text << '\n';
            }
            #endif
            break;
        }
    }
private:
    std::unordered_set<std::string> unique_;
};



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
    : original_(origin)
    , current_(origin)
    , handler_(handler) {}

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

template <typename OriginalIterator>
NotifyingIterator<OriginalIterator>
makeNotifyingIterator(OriginalIterator& original, IEventHandler& handler) {
    return NotifyingIterator<OriginalIterator>(original, handler);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (const auto& x: vec) {
        os << x << ", ";
    }
    return os;
}


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
    auto begin_ = NotifyingIterator(data.begin(), handler);
    auto end_ = NotifyingIterator(data.end(), handler);
    std::sort(begin_, end_);
}

void partitionCase(IEventHandler& handler) {
    auto data = getMonotonic(10);
    auto begin_ = NotifyingIterator(data.begin(), handler);
    auto end_ = NotifyingIterator(data.end(), handler);
    std::partition(begin_, end_, [] (int x) { return x % 3; });
}


int main() {
    DebugLogger log;
    //sortCase(log);
    partitionCase(log);
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

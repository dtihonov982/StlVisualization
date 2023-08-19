#include <unordered_map>
#include <iostream>

class Foo {
public:
    Foo() {}
    Foo(const Foo&) {
        std::cout << "Copy ctor\n";
    }
    Foo(Foo&&) {
        std::cout << "Move ctor\n";
    }
};

int main() {
    std::unordered_map<int, Foo> M;
    M.emplace(0, Foo{});
    return 0;
}

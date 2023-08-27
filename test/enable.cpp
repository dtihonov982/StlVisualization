#include <type_traits>
#include <iostream>

struct A {
    void foo() {}
};

struct B {
    void foo() {}
    void bar() {}
};

template <typename T>
struct X {
    X(T& f): f_(f) {}
    void foo() { f_.foo(); }
    void bar() { f_.bar(); }
    T f_;
};

int main() {
    A a;
    B b;
    X<A> xa(a);
    xa.foo();
    X<B> xb(b);
    xb.foo();
    xb.bar();
    return 0;
}

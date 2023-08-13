#include <fstream>
#include <memory>

class Test {
public:
    Test(std::ofstream&& file)
    : file_(std::move(file)) {}
    void test() { file_ << "success!"; }
    std::ofstream file_;
};

int main() {
    std::ofstream file{"test"};
    Test t{std::move(file)};
    t.test();


    return 0;
}

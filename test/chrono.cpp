#include <chrono>

int main() {
        auto start = std::chrono::high_resolution_clock::now();
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(start - finish);
        auto count = duration.count();
        return 0;
}

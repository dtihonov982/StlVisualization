#include <iostream>
#include <chrono>
#include <thread>
 
int main()
{
    using namespace std::chrono_literals;
 
    std::cout << "Hello waiter\n" << std::flush;
 
    const auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(2000ms);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = end - start;
 
    std::cout << "Waited " << elapsed.count() << '\n';
}

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

std::atomic<int> counter(0);    // shared atomic variable
// good for ++ or flags
// for complex stuff still suggest to use mutex

void increment(){
    for (int i = 0; i < 100000; ++i){
        ++counter;
    }
}

int main() {
    std::vector<std::thread> threads;
    threads.emplace_back(increment);
    threads.emplace_back(increment);

    for (std::thread &t : threads){
        if (t.joinable()){
            t.join();
        }
    }

    std::cout << "Counter (expected 200000): " << counter << "\n";

    return 0;
}
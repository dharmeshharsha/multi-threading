// reduces number of lock/unlock operations drastically — better throughput.
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

int counter = 0;    // shared non-atomic variable
std::mutex mtx;

// Reduce locking overhead by accumulating locally then locking once
void increment(){
    int local = 0;
    for (int i = 0; i < 100000; ++i){
        ++local;
    }
    {
        std::lock_guard<std::mutex> lg(mtx);
        counter += local;
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
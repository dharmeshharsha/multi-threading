#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

int counter = 0;    // shared non-atomic variable
std::mutex mtx;

void increment(){
    for (int i = 0; i < 100000; ++i){
        std::lock_guard<std::mutex> lg(mtx);
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
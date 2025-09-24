#include <iostream>
#include <thread>
#include <vector>

int counter = 0;    // shared non-atomic variable

void increment(){
    for (int i = 0; i < 100000; ++i){
        ++counter;  // data race when multiple threads run this concurrently
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
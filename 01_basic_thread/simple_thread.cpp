#include <iostream>
#include<vector>
#include<thread>

void worker(int id){
    std::cout << "Hello from worker <" << id << ">\n";
}

int main() {
    const int num = 4; // number of threads

    std::vector<std::thread> threads;   // containers to keep thread objects
    //threads.reserve(num);   // optional: avoid reallocations
    for (int i = 0; i < num; ++i){
        threads.push_back(std::thread(worker, i+1));
    }

    for (std::thread &t : threads){
        if (t.joinable()){t.join();}
    }

    std::cout << "All threads joined!\n";
    return 0;
}
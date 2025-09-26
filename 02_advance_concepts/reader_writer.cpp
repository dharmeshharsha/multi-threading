//reader_writer.cpp
/*
IDEA: 
- Many threads reads the same data frequently; occasionally one thread writes (modifies it)
- Readers can safely run concurrently with other readers
- Writers needs exclusive access (no other reader or writer at the same time)
- std::shared_mutex (C++17) implements this: it supports shared (read) lock and exclusive (write) lock
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>


int shared_data = 2;

std::shared_mutex rw_mutex;      // reader-writer mutex
std::mutex cout_mutex;           // protect std::cout from interleaving (mixing of 2 std::cout)
std::atomic<int> active_readers(0);

void reader(int id, int iterations) {
    for (int k = 0; k < iterations; k++) {
        std::shared_lock<std::shared_mutex> lock(rw_mutex);
        active_readers ++;
        // Reading
        {// because cout is also not thread safe - could have interleaving
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << "Reader <" << id << "> is reading and Shared data is: " << shared_data << " | active_readers=" << active_readers << "\n";
        }

        // lets add some reading time
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        active_readers --;
    }
}


void writer(int id, int iterations) {
    for (int j = 0; j < iterations; j++) {
        std::unique_lock<std::shared_mutex> lock(rw_mutex);
        // Writing
        shared_data = shared_data * 2;
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << "Writer <" << id << "> is writing and upd. shared_data is :" << shared_data << " | active_readers=" << active_readers << "\n";
        }
    }
}


int main() {

    const int num_reader = 4;
    const int num_writer = 2;

    std::vector<std::thread> threads;
    
    // writer class's threads
    for (int i = 0; i < num_reader; ++i) {
        threads.emplace_back(reader, i, 5);
    }

    for (int i = 0; i < num_writer; i++) {
        threads.emplace_back(writer, i, 8);
    }

    for (std::thread& t : threads) {
        if (t.joinable()) {t.join();}
    }

    std::cout << "Final shared_data is : " << shared_data  << "\n";
    return 0;

}


// clang++ -std=c++17 -pthread reader_writer.cpp -o reader_writer
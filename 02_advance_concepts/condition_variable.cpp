// Classic producer-consumer problem
#include <iostream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>


std::queue<int> q;
std::mutex m;
std::condition_variable cv;
bool done = false;

void producer() {
    for (int i = 0; i < 10; ++i) {
        {
            std::lock_guard<std::mutex> lg(m);
            q.push(i);
        }
        cv.notify_one();
    }
    {
        std::lock_guard<std::mutex> lg(m);
        done = true;
    }
    cv.notify_all();
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, []{return (!q.empty() || done); }); //until the condition is satisfied, it waits
        // while it waits, it unlocks for other to access the shared resource

        if (!q.empty()) {
            int item = q.front(); q.pop();
            std::cout << "Consumed: <" << item << ">\n";
        }
        else if (done) break;
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}

/* 
1. Here you should note that we have used std::lock_guard for producer
but used the std::unique_lock for consumer thread. Check for the reason

2. Moreover think about what is the added benifit for using this condition variable
If only mutex was able to solve the issue - Maybe something related to busy-waiting or sleep-an-check
*/
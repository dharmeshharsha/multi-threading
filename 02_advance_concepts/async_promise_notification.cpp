/*
This is an example of how aync+future and promise+future interact with each other in real life

Scenario:
Imagine you have to do some heavy task like read a large doc and do some calculations based on it.
You dont want your main program to just wait for it. Rather it should be able to do other stuff
Like maybe some loading screen, being able to take inputs from user, etc.

So here, we use async and future to let this heavy long work to be done in another thread, and
ask it to let me know once its done.
But the catch is: should we use a shared resource where the result will be stored from heavy task?
OR can we have an alternative of promise. Promise that once the work is done, future will get the results (in an internal shared resource which only both of them know)

In this way we got rid of lock and mutex, coz thats already taken care of.

*/


#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>

// Function to compute square of number
int calculate_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return x * x;
}

// 
void notifier(std::promise<void> done_signal, int num_tasks) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Notify the main thread\n";
    done_signal.set_value();    // Notify the main thread
}

int main() {
    std::vector<int> numbers = {1,2,3,4,5};
    std::vector<std::future<int>> f;

    // launch async tasks for calculating the squares
    for (int n : numbers) {
        f.push_back(std::async(std::launch::async, calculate_square, n));        
    }

    // setup a promise/future combo to notify when everthing is done
    std::promise<void> done_promise;
    std::future<void> done_future = done_promise.get_future();

    // start a new thread named "notify_thread"
    std::thread notify_thread(notifier, std::move(done_promise), numbers.size());


    // collect the results from the async tasks of squaring the numbers
    for (int i = 0; i < f.size(); i++) {
        int result = f[i].get();
        std::cout << "Square of {" numbers[i] "} is {" << result << "}\n";
    }

    done_future.get();
    std::cout << "All things are done\n";

    notify_thread.join();

    return 0;
}
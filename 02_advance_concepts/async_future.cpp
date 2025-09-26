/*Intuition
If you want to run a function in background (separate thread) and get its result later.
Why would we want? Because we dont want to wait on that particular statement,
rahter we want to contunue with the next statements in the current thread
until we come to the statement where we really require the output of the function which is running on other thread

Solution is using future with async
*/

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int compute_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));  //sleep 2 ms
    return (x * x);
}

int main() {
    std::cout << "Start async task\n";

    std::future<int> fut = std::async(std::launch::async, compute_square, 5);

    std::cout << "Doing other work in the main thread...\n";

    //Get the (wait until the compute_square finishes)
    int result = fut.get();

    std::cout << "Result is " << result << "\n";

    return 0;
}

// here std::launch::async forces to start a new thread for running compute_square
// fut.get() will only wait if result isn't ready yet
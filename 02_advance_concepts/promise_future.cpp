/*
Here we will first define a promise of type of the output of the function
Pass this promise as an argument variable into the function
*/


#include <iostream>
#include <thread>
#include <future>


void worker(std::promise<int> p) {
    int value = 42;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    p.set_value(value);
}

int main() {
    std::promise<int> p;                    // promise is defined
    std::future fut = p.get_future();       // we tell the future that we promise to provide the output via p
    
    std::thread t(worker, std::move(p));               // why std::move(p) and not just p, try with just p -> it doesn't have a copy constructor
    
    std::cout << "Waiting for the value in worker\n";
    int result = fut.get();
    std::cout << "Result : " << result << "\n";

    t.join();

    return 0;
}

/*
I got it that promise doesn't have a copy constructor so we need to move it instead. 
Firstly why is so? There must some intention behind not allowing it to be copied. 
promise is made of some data type (mostly for returning), and then this object is passed into the other thread where it will be updated. 
Then how is it still accessible from the main thread? Does it because before moving into other thread, we linked it with the future. 
So there must some address level pointing/linking of the promise to the future?

Anwser to why not copyable -> If so, then more than one promise object trying to set the same future, which in undefined behaviour
single onwership at compile time

Awnser to how is moved promise still able to return value to future ->
std::future fut = p.get_future(); This line makes an internal shared state (hidden managed by std libs) 
This state is on heap, not on stack of p or fut
*/

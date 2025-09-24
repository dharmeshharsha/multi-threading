Files:
- `simple_thread.cpp` - spawn/join multiple threads
- `race_condition.cpp` - data race demonstration
- `mutex_solution.cpp` - fix with std::mutex
- `optimized_mutex_solution.cpp` - reduce lock overhead
- `atomic_counter.cpp` - lock-free counter using std::atomic

Build:
  clang++ -pthread <file>.cpp -o <binary>

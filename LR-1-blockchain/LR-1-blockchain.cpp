//Демонстрация ошибки race condition.

#include <iostream>
#include <thread>

static long long counter = 0;

void inc(long long n) {
    for (long long i = 0; i < n; ++i) {
        // НЕатомарная операция: read-modify-write
        counter++;
    }
}

int main() {
    const long long N = 10'000'000;

    std::thread t1(inc, N);
    std::thread t2(inc, N);

    t1.join();
    t2.join();

    std::cout << "Expected: " << (2 * N) << "\n";
    std::cout << "Actual:   " << counter << "\n";
    return 0;
}

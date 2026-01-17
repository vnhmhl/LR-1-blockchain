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



//Исправление ошибки через mutex
#include <iostream>
#include <thread>
#include <mutex>

static long long counter = 0;
static std::mutex m;

void inc(long long n) {
    for (long long i = 0; i < n; ++i) {
        std::lock_guard<std::mutex> lock(m);
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



//Исправление ошибки через atomic
#include <iostream>
#include <thread>
#include <atomic>

static std::atomic<long long> counter{ 0 };

void inc(long long n) {
    for (long long i = 0; i < n; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    const long long N = 10'000'000;

    std::thread t1(inc, N);
    std::thread t2(inc, N);

    t1.join();
    t2.join();

    std::cout << "Expected: " << (2 * N) << "\n";
    std::cout << "Actual:   " << counter.load() << "\n";
    return 0;
    

    
    //Демонстрация ошибки Deadlock.
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

    std::mutex m1, m2;

    void t1() {
        std::lock_guard<std::mutex> lock1(m1);
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // усиливаем шанс дедлока
        std::lock_guard<std::mutex> lock2(m2);
        std::cout << "t1 finished\n";
    }

    void t2() {
        std::lock_guard<std::mutex> lock1(m2);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::lock_guard<std::mutex> lock2(m1);
        std::cout << "t2 finished\n";
    }

    int main() {
        std::thread a(t1);
        std::thread b(t2);

        a.join();
        b.join();

        std::cout << "Done\n";
        return 0;
    }

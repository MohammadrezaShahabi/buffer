#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>
#include <condition_variable>

const int BUFFER_SIZE = 10; // size of buffer
std::queue<int> buffer; // buffer
std::mutex mtx; // mutex for synchronizing access to buffer
std::condition_variable cv; // condition variable for synchronizing producer and consumer

void producer() {
    for (int i = 0; i < 20; ++i) {
        std::unique_lock<std::mutex> lck(mtx);
        while (buffer.size() == BUFFER_SIZE) { // if buffer is full, wait until signaled
            cv.wait(lck);
        }
        buffer.push(i); // add new item to buffer
        std::cout << "Producer produced " << i << std::endl;
        cv.notify_all(); // signal all waiting threads
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // add delay for testing
    }
}

void consumer() {
    for (int i = 0; i < 20; ++i) {
        std::unique_lock<std::mutex> lck(mtx);
        while (buffer.empty()) { // if buffer is empty, wait until signaled
            cv.wait(lck);
        }
        int val = buffer.front(); // buffer size is greater than zero, so this condition always holds
        buffer.pop(); // remove first item from buffer
        std::cout << "Consumer consumed " << val << std::endl;
        cv.notify_all(); // signal all waiting threads
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); // add delay for testing
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}

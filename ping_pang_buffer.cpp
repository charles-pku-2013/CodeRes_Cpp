#include <array>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

constexpr int BUFFER_SIZE = 5;
constexpr int ROUND_COUNT = 5;
using Buffer = std::array<int, BUFFER_SIZE>;

Buffer buffers[2];

// ready[i] == true：buffers[i] 中有数据，消费者正在等待或读取它。
// ready[i] == false：buffers[i] 是空的，生产者可以写入它。
bool ready[2] = {false, false};

std::mutex bufferMutex;
std::condition_variable canWrite;
std::condition_variable canRead;

void writeData(Buffer& buffer, int round) {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = round * 10 + i;
    }
}

void readData(const Buffer& buffer, int round) {
    std::cout << "第 " << round << " 轮读取：";
    for (int value : buffer) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}

void producer() {
    int writeIndex = 0;

    for (int round = 1; round <= ROUND_COUNT; ++round) {
        // 等待当前缓冲区变为空闲。
        {
            std::unique_lock<std::mutex> lock(bufferMutex);
            canWrite.wait(lock, [writeIndex] {
                return !ready[writeIndex];
            });
        }

        // 只有 producer 会写这个空闲缓冲区，所以写入时不需要持有锁。
        writeData(buffers[writeIndex], round);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // 写完后，将缓冲区标记为“有数据”。
        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            ready[writeIndex] = true;
        }
        canRead.notify_one();

        // 下次写另一个缓冲区：0 -> 1 -> 0 -> 1 ...
        writeIndex = 1 - writeIndex;
    }
}

void consumer() {
    int readIndex = 0;

    for (int round = 1; round <= ROUND_COUNT; ++round) {
        // 等待当前缓冲区中出现数据。
        {
            std::unique_lock<std::mutex> lock(bufferMutex);
            canRead.wait(lock, [readIndex] {
                return ready[readIndex];
            });
        }

        // 保持 ready 为 true，防止 producer 覆盖正在读取的数据。
        readData(buffers[readIndex], round);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // 读完后，将缓冲区标记为“空闲”。
        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            ready[readIndex] = false;
        }
        canWrite.notify_one();

        // 下次读另一个缓冲区：0 -> 1 -> 0 -> 1 ...
        readIndex = 1 - readIndex;
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    std::cout << "处理完成\n";
    return 0;
}


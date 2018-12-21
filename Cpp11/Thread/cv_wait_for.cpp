#include <iostream>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

#define KNN_TIMEOUT 20000   // 20ms

struct KnnResult {
    using pointer = std::shared_ptr<KnnResult>;

    explicit KnnResult(int k) : done_(false) {
        scores.assign(k, 0.0);
        labels.assign(k, -1);
    }

    bool wait_result(int timeout) {
        std::unique_lock<std::mutex> lk(mtx);
        if (timeout > 0) {
            return cv.wait_for(lk, std::chrono::microseconds(timeout), []{return done();});
        } else {
            cv.wait(lk, [&, this]{return done();});
        }
        return true;
    }

    void notify_result() {
        done_ = true;
        cv.notify_all();
    }

    bool done() const { return done_; }

    std::atomic_bool done_;
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<float> scores;
    std::vector<int> labels;
};

int main() {
    KnnResult k(5);

    return 0;
}

// using namespace std::literals::chrono_literals;
 
// std::condition_variable cv;
// std::mutex cv_m;
// int i;
 
// void waits(int idx)
// {
    // std::unique_lock<std::mutex> lk(cv_m);
    // if(cv.wait_for(lk, idx*100ms, []{return i == 1;})) 
        // std::cerr << "Thread " << idx << " finished waiting. i == " << i << '\n';
    // else
        // std::cerr << "Thread " << idx << " timed out. i == " << i << '\n';
// }
 
// void signals()
// {
    // std::this_thread::sleep_for(120ms);
    // std::cerr << "Notifying...\n";
    // cv.notify_all();
    // std::this_thread::sleep_for(100ms);
    // {
        // std::lock_guard<std::mutex> lk(cv_m);
        // i = 1;
    // }
    // std::cerr << "Notifying again...\n";
    // cv.notify_all();
// }
 
// int main()
// {
    // std::thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals);
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
// }

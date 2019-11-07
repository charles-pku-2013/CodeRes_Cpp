#include "common/periodic/periodic_task.h"
#include <glog/logging.h>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <iostream>
#include "gtest/gtest.h"

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))

using std::endl;
using std::cout;
using std::cin;

class PeriodicTest : public testing::Test {
 public:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

struct Printer : public PeriodicTask {
    explicit Printer(int _id) : id(_id) {}
    void RunSchedule() override {
        cout << THIS_THREAD_ID << " Printer #" << id << " run " << ++count << endl;;
        // SLEEP_SECONDS(120);
    }
    int count = 0;
    int id = 0;
};

// void test_PeriodicTaskSet() {
TEST_F(PeriodicTest, t1) {
    // auto period_task_set = std::make_shared<PeriodicTaskSet>("Test");
    // period_task_set->Start(2);
    PeriodicService::Instance().NewPeriodicTaskSet("Test", 2);
    auto period_task_set = PeriodicService::Instance().Get("Test");
    assert(period_task_set);
    auto p_task1 = std::make_shared<Printer>(1);
    auto p_task2 = std::make_shared<Printer>(2);
    period_task_set->AddTask("Printer1", p_task1);
    period_task_set->AddTask("Printer2", p_task2);
    cout << period_task_set->DebugString() << endl;
    cin.get();
    cout << "Destroying task1..." << endl;
    p_task1.reset();
    cin.get();
    cout << "Stopping timer..." << endl;
    period_task_set->Stop();
    cin.get();
    cout << "Start timer again..." << endl;
    period_task_set->Start(5);
    cin.get();
    cout << "END TEST" << endl;
}

// int main(int argc, char **argv) {
    // google::InitGoogleLogging(argv[0]);

    // test_PeriodicTaskSet();

    // return 0;
// }



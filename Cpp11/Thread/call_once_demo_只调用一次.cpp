#include <iostream>
#include <thread>
// #include <mutex>
 
std::once_flag flag;
 
void do_once()
{
    std::call_once(flag, [](){ std::cout << "Called once" << std::endl; });
}
 
int main()
{
    do_once();
    do_once();
    do_once();
    do_once();
    do_once();
    exit(0);


    std::thread t1(do_once);
    std::thread t2(do_once);
    std::thread t3(do_once);
    std::thread t4(do_once);
 
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

#if 0
namespace example1 {
std::once_flag tflite_session_init_flag;
if (predictor->IsTfLite()) {
    tensorflow::Status tflite_init_status;
    std::call_once(tflite_session_init_flag, [&](tensorflow::Status *status){
        *status = tensorflow::serving::TfLiteSession::Init(predictor->InnerModelName(), predictor->Version(),
                predictor->BatchSize());
    }, &tflite_init_status);
    if (!tflite_init_status.ok()) {
        LOG(ERROR) << "BaseLTFRank::RankImpl TfLiteSession init fail! " << tflite_init_status;
        return;
    }
}
}  // namespace example1
#endif


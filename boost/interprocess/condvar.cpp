#include <boost/interprocess/managed_shared_memory.hpp> 
#include <boost/interprocess/sync/interprocess_mutex.hpp> 
#include <boost/interprocess/sync/interprocess_condition.hpp> 
#include <boost/interprocess/sync/scoped_lock.hpp> 
#include <iostream> 

using namespace boost::interprocess;

// 1. 定义共享数据结构
struct SharedData {
    interprocess_mutex mutex;
    interprocess_condition cond_var;
    std::string message;
    bool data_ready = false;
};

int main() {
    // 假设这是子进程（消费者），父进程（生产者）负责创建共享内存
    // managed_shared_memory 负责管理共享内存的生命周期
    managed_shared_memory segment(open_only, "MySharedMemory");

    // 2. 查找共享内存中的数据对象
    SharedData* data = segment.find<SharedData>("SharedDataObject").first;

    // 3. 使用 Mutex 和 Condition Variable 进行同步
    {
        // 锁定互斥体
        scoped_lock<interprocess_mutex> lock(data->mutex);

        // 如果数据还没准备好，等待条件变量信号
        while (!data->data_ready) {
            std::cout << "Waiting for data..." << std::endl;
            data->cond_var.wait(lock); // 等待期间会自动释放锁，被唤醒后会自动重新获取锁
        }

        // 数据已准备好，安全读取
        std::cout << "Received message: " << data->message << std::endl;
        data->data_ready = false; // 重置状态
    } // 离开作用域，锁自动释放

    return 0;
}

int main() 
{ 
  try 
  { 
    boost::interprocess::managed_shared_memory managed_shm(boost::interprocess::open_or_create, "shm", 1024); 
    int *i = managed_shm.find_or_construct<int>("Integer")(0); 
    boost::interprocess::interprocess_mutex *mtx = managed_shm.find_or_construct<boost::interprocess::interprocess_mutex>("mtx")(); 
    boost::interprocess::interprocess_condition *cnd = managed_shm.find_or_construct<boost::interprocess::interprocess_condition>("cnd")(); 
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(*mtx); 
    while (*i < 10) 
    { 
      if (*i % 2 == 0) 
      { 
        ++(*i); 
        cnd->notify_all(); 
        cnd->wait(lock); 
      } 
      else 
      { 
        std::cout << *i << std::endl; 
        ++(*i); 
        cnd->notify_all(); 
        cnd->wait(lock); 
      } 
    } 
    cnd->notify_all(); 
  } 
  catch (...) 
  { 
  } 
  boost::interprocess::shared_memory_object::remove("shm"); 
} 

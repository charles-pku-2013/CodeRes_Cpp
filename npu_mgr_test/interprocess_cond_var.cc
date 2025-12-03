#ifndef SHARED_DATA_BOOST_H
#define SHARED_DATA_BOOST_H

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

struct SharedDataBoost {
    boost::interprocess::interprocess_mutex mutex;      // 互斥锁
    boost::interprocess::interprocess_condition cond;    // 条件变量
    int data;                                           // 共享数据
    bool produced;                                      // 标记数据是否已生产
};

#endif // SHARED_DATA_BOOST_H

// producer
#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <unistd.h>
#include "shared_data_boost.h"

namespace bip = boost::interprocess;

int main() {
    const char* SHM_NAME = "BoostSharedMemoryExample";

    try {
        // 1. 创建或打开一个托管的共享内存段
        // SegmentManager 会自动在共享内存中创建并初始化 SharedDataBoost 结构
        bip::managed_shared_memory segment(bip::create_only, SHM_NAME, 65536);

        // 2. 在共享内存中构造 SharedDataBoost 对象
        SharedDataBoost* shared_data = segment.construct<SharedDataBoost>("SharedDataInstance")();

        shared_data->produced = false;
        
        std::cout << "Producer: 共享内存已创建，开始生产数据..." << std::endl;

        // 3. 生产数据循环
        for (int i = 0; i < 5; ++i) {
            // 使用 scoped_lock 自动管理互斥锁的加锁和解锁
            bip::scoped_lock<bip::interprocess_mutex> lock(shared_data->mutex);

            // 等待条件：直到消费者取走数据
            while (shared_data->produced) {
                std::cout << "Producer: 等待消费者..." << std::endl;
                // 原子地释放锁并等待 cond 信号
                shared_data->cond.wait(lock);
            }

            shared_data->data = i * 100;
            shared_data->produced = true;
            std::cout << "Producer: 生产数据 -> " << shared_data->data << std::endl;

            // 通知等待条件的消费者
            shared_data->cond.notify_one(); 
            
            // scoped_lock 在这里超出作用域，自动解锁（或者循环开始时手动解锁）
        }

        sleep(1); // 留点时间让消费者完成最后一次读取
        std::cout << "Producer: 生产完成。" << std::endl;

    } catch (bip::interprocess_exception &ex) {
        std::cerr << "Producer Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

// consumer
#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <unistd.h>
#include "shared_data_boost.h"

namespace bip = boost::interprocess;

int main() {
    const char* SHM_NAME = "BoostSharedMemoryExample";

    try {
        // 1. 打开已存在的托管共享内存段
        bip::managed_shared_memory segment(bip::open_only, SHM_NAME);

        // 2. 查找共享内存中的 SharedDataBoost 实例
        SharedDataBoost* shared_data = segment.find<SharedDataBoost>("SharedDataInstance").first;
        
        if (!shared_data) {
            std::cerr << "Consumer Error: 无法找到共享数据实例。" << std::endl;
            return 1;
        }

        std::cout << "Consumer: 连接到共享内存，开始消费数据..." << std::endl;

        // 3. 消费数据循环
        for (int i = 0; i < 5; ++i) {
            bip::scoped_lock<bip::interprocess_mutex> lock(shared_data->mutex);

            // 等待条件：直到生产者生产数据
            while (!shared_data->produced) {
                std::cout << "Consumer: 等待生产者..." << std::endl;
                shared_data->cond.wait(lock);
            }

            std::cout << "Consumer: 消费数据 <- " << shared_data->data << std::endl;
            shared_data->produced = false;

            // 通知生产者可以继续生产了
            shared_data->cond.notify_one();

            // scoped_lock 自动解锁
        }

        std::cout << "Consumer: 消费完成，清理共享内存。" << std::endl;
        // 4. 清理：删除共享内存对象，以便下次运行程序时能重新创建
        bip::shared_memory_object::remove(SHM_NAME);

    } catch (bip::interprocess_exception &ex) {
        // 如果消费者先运行会报错（找不到 SHM），或者其他异常
        std::cerr << "Consumer Error: " << ex.what() << std::endl;
        // 确保清理
        bip::shared_memory_object::remove(SHM_NAME);
        return 1;
    }

    return 0;
}

/*
 * c++ -o /tmp/test shared_lockable.cpp -lboost_thread -lboost_system -lglog -std=c++11 -g
 * GLOG_logtostderr=1 /tmp/test
 *
 * lockable_adapter 继承关系
 * basic_lockable_adapter <-
 * lockable_adapter <-
 * timed_lockable_adapter <- (Only support try_lock_for and try_lock_until)
 * shared_lockable_adapter <-
 * upgrade_lockable_adapter
 *
 *
 */
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/date_time.hpp>
#include <ctime>
#include <iostream>
#include <deque>
#include <functional>
#include <glog/logging.h>

#define THIS_THREAD_ID        boost::this_thread::get_id()
#define SLEEP_SECONDS(x)      boost::this_thread::sleep_for(boost::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) boost::this_thread::sleep_for(boost::chrono::milliseconds(x))

using std::cin; using std::cout; using std::endl;

/*
 * 继承关系图: http://charette.no-ip.com:81/programming/doxygen/boost/classboost_1_1shared__lockable__adapter.html
 * upgrade_lockable_adapter 继承自 upgrade_lockable_adapter
 */
struct SafeList : public std::deque<std::string>
                , public boost::upgrade_lockable_adapter<boost::shared_mutex> 
{

};


static
void reader( SafeList &sl )
{
    LOG(INFO) << "Reader thread " << THIS_THREAD_ID << " running...";
    uint32_t sleepTime = 100 + rand() % 900;
    SLEEP_MILLISECONDS(sleepTime);    // let Writer run first

    boost::shared_lock<SafeList> lock(sl);
    LOG(INFO) << THIS_THREAD_ID << " Reader got the shared lock";

    // pretend doing something
    SLEEP_SECONDS(3);

    LOG(INFO) << THIS_THREAD_ID << " Reader done!";
}

static
void writer( SafeList &sl )
{
    LOG(INFO) << "Writer thread " << THIS_THREAD_ID << " running...";

    //!! NOTE!! lock 的模板参数是继承类名而不是基类 lockable
    boost::upgrade_lock<SafeList> lock(sl);
    LOG(INFO) << "Writer thread " << THIS_THREAD_ID << " has got the shared lock.";
    boost::upgrade_to_unique_lock<SafeList>  unique_lock(lock);
    LOG(INFO) << "Writer thread " << THIS_THREAD_ID << " has got the unique lock.";
    
    // pretend doing something
    SLEEP_SECONDS(3);

    LOG(INFO) << THIS_THREAD_ID << " Writer done!";
}


int main( int argc, char **argv )
{
    google::InitGoogleLogging(argv[0]);

    SafeList sl;

    srand( time(0) );
    boost::thread thrReader1( std::bind(reader, std::ref(sl)) );
    boost::thread thrReader2( std::bind(reader, std::ref(sl)) );
    boost::thread thrWriter1( std::bind(writer, std::ref(sl)) );

    cin.get();

    return 0;
}


#if 0
// timed lock example:
bool setValue(const std::string &id, const double &v)
{
    auto &table = m_mapTable[id[0]][id[1]];
    boost::unique_lock<DbMap> lockTable(table, boost::defer_lock);
    lockTable.try_lock_for(boost::chrono::seconds(LOCK_TIMEOUT));
    if (lockTable.owns_lock()) {
        auto it = table.find(id);
        if (it == table.end())
            return false;
        boost::unique_lock<ValueArray> lockArr(m_arrHasValue, boost::defer_lock);
        lockArr.try_lock_for(boost::chrono::seconds(LOCK_TIMEOUT));
        if (lockArr.owns_lock()) {
            Item::pointer pItem = it->second;
            table.erase(it);
            pItem->setValue(v);
            m_arrHasValue.push_back(pItem);
            return true;
        } // if
    } // if
    return false;
}
#endif

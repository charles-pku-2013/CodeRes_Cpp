#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include <vector>

boost::shared_mutex g_mutex;

using namespace std;

/*
 * void reader(string id)
 * {
 *     cout << id << ": running" << endl;
 * 
 *     boost::shared_lock<boost::shared_mutex> lock(g_mutex);
 *     if (!lock.owns_lock())
 *         cout << id << ": Writer has the lock" << endl;
 *     else
 *         cout << id << ": Got the lock" << endl;
 * 
 *     // pretend doing something
 *     boost::this_thread::sleep(boost::posix_time::seconds(3));
 * 
 *     cout << id << ": finished" << endl;
 * }
 * 
 * void writer(string id)
 * {
 *     cout << id << ": running" << endl;
 * 
 *     boost::upgrade_lock<boost::shared_mutex> lock(g_mutex);
 *     boost::upgrade_to_unique_lock<boost::shared_mutex>  unique_lock(lock);
 * 
 *     cout << id << ": Got the lock" << endl;
 * 
 *     // pretend doing something
 *     boost::this_thread::sleep(boost::posix_time::seconds(3));
 * 
 *     cout << id << ": finished" << endl;
 *     boost::this_thread::sleep(boost::posix_time::seconds(1));
 * }
 * 
 * int main(int argc, char* argv[])
 * {
 *     const int writerthreadcount = 2;
 *     const int readerthreadcount = 2;
 * 
 *     vector<boost::thread*> writerthread;
 *     vector<boost::thread*> readerthread;
 * 
 *     for(int i=0; i<writerthreadcount; i++){
 *         ostringstream id;  id << "writer" << i+1;
 *         writerthread.push_back(new boost::thread(writer, id.str()));
 *     }
 *     for(int i=0; i<readerthreadcount; i++){
 *         ostringstream id;  id << "reader" << i+1;
 *         readerthread.push_back(new boost::thread(reader, id.str()));
 *     }
 * 
 * //    boost::this_thread::sleep(boost::posix_time::seconds(1));
 * 
 *     cout << "main: waiting for thread" << endl;
 * 
 *     for(int i=0; i<writerthreadcount; i++){
 *         writerthread[i]->join(); delete writerthread[i];
 *     }
 *     for(int i=0; i<readerthreadcount; i++){         
 *         readerthread[i]->join(); delete readerthread[i];
 *     }
 * 
 *     cout << "main: done" << endl;
 * 
 *     return 0;
 * }
 */


// 一般用法:
boost::shared_mutex _access;
void reader()
{
  boost::shared_lock< boost::shared_mutex > lock(_access);
  // do work here, without anyone having exclusive access
}

void conditional_writer()
{
  boost::upgrade_lock< boost::shared_mutex > lock(_access);
  // do work here, without anyone having exclusive access

  if (something) {
    boost::upgrade_to_unique_lock< boost::shared_mutex > uniqueLock(lock);
    // do work here, but now you have exclusive access
  }

  // do more work here, without anyone having exclusive access
}

void unconditional_writer()
{
  boost::unique_lock< boost::shared_mutex > lock(_access);
  // do work here, with exclusive access
}


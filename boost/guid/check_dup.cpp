//  (C) Copyright Andy Tompkins 2009. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  libs/uuid/test/test_tagging.cpp  -------------------------------//

#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/format.hpp>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>

typedef boost::uuids::uuid	UUID;

inline
std::ostream& operator << (std::ostream &os, const UUID &uuid)
{
	os << boost::uuids::to_string(uuid);
	return os;
}

template <typename T>
class ThreadSafeSet : private std::set<T>
                    , public boost::shared_lockable_adapter<boost::shared_mutex>
{
private:
    typedef typename std::set<T>    BaseType;
public:
    std::pair<typename ThreadSafeSet::iterator, bool>
    insert(const T &val)
    {
        boost::unique_lock<ThreadSafeSet> lk(*this);
        return BaseType::insert(val);
    }

    std::size_t size()
    {
        boost::shared_lock<ThreadSafeSet> lk(*this);
        return BaseType::size();
    }
};


ThreadSafeSet<UUID> g_IdSet;

int main(int, char*[])
try {
    using namespace std;

    auto n = boost::thread::hardware_concurrency();

    boost::thread_group thrgroup;

    for( int i = 0; i < n; ++i )
        thrgroup.create_thread([&]{
            while (true) {
                UUID uid((boost::uuids::random_generator())());
                auto ret = g_IdSet.insert(uid);
                if (!ret.second) {
                    cout << uid << " already exists! set size is " << g_IdSet.size() << endl;
                } // if
            } // while
        });

    thrgroup.join_all();
    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
}

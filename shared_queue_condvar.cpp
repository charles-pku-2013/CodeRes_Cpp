#include <deque>
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/chrono.hpp>

template < typename T >
class SharedQueue : private std::deque<T> {
    typedef typename std::deque<T>   BaseType;
public:
    SharedQueue( std::size_t _MaxSize = UINT_MAX ) 
                : maxSize(_MaxSize) {}

    bool full() const { return this->size() >= maxSize; }

    void push( const T& elem )
    {
        boost::unique_lock<boost::mutex> lk(lock);

        while( this->full() )
            condWr.wait( lk );

        this->push_back( elem );

        lk.unlock();
        condRd.notify_one();
    }

    void push( T&& elem )
    {
        boost::unique_lock<boost::mutex> lk(lock);

        while( this->full() )
            condWr.wait( lk );

        this->push_back( std::move(elem) );

        lk.unlock();
        condRd.notify_one();
    }

    void pop( T& retval )
    {
        boost::unique_lock<boost::mutex> lk(lock);
        
        while( this->empty() )
            condRd.wait( lk );

        retval = std::move(this->front());
        this->pop_front();

        lk.unlock();
        condWr.notify_one();
    }

    T pop()
    {
        T retval;
        this->pop( retval );
        return retval;
    }

    bool timed_push( const T& elem, std::size_t timeout )
    {
        boost::unique_lock<boost::mutex> lk(lock);

        if (!condWr.wait_for(lk, boost::chrono::milliseconds(timeout),
                    [this]()->bool {return !this->full();}))
            return false;

        this->push_back( elem );

        lk.unlock();
        condRd.notify_one();

        return true;
    }

    bool timed_push( T&& elem, std::size_t timeout )
    {
        boost::unique_lock<boost::mutex> lk(lock);

        if (!condWr.wait_for(lk, boost::chrono::milliseconds(timeout),
                    [this]()->bool {return !this->full();}))
            return false;

        this->push_back( std::move(elem) );

        lk.unlock();
        condRd.notify_one();

        return true;
    }

    bool timed_pop( T& retval, std::size_t timeout )
    {
        boost::unique_lock<boost::mutex> lk(lock);
        
        //!! 这里的pred条件和while正好相反，相当于until某条件
        if (!condRd.wait_for(lk, boost::chrono::milliseconds(timeout),
                  [this]()->bool {return !this->empty();}))
            return false;

        retval = std::move(this->front());
        this->pop_front();

        lk.unlock();
        condWr.notify_one();

        return true;
    }

    void clear()
    {
        boost::unique_lock<boost::mutex> lk(lock);
        BaseType::clear();
    }

protected:
    const std::size_t             maxSize;
    boost::mutex                  lock;
    boost::condition_variable     condRd;
    boost::condition_variable     condWr;
};


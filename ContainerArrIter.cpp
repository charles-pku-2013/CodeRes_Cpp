#include <iostream>
#include <vector>
#include <list>
#include <map>


/*
 * template< typename ArrType, typename Container, int END_IDX, int START_IDX = 0 >
 * struct ContainerArrIter : 
 *         std::iterator< typename std::iterator_traits<typename Container::iterator>::iterator_category,
 *                     typename Container::value_type >
 */
template< typename ArrType, typename Container, int END_IDX, int START_IDX = 0 >
struct ContainerArrIter : 
        std::iterator< std::forward_iterator_tag, typename Container::value_type >
{
    typedef std::iterator< std::forward_iterator_tag, 
                    typename Container::value_type >  BaseType;
    typedef typename BaseType::iterator_category      iterator_category;
    typedef typename BaseType::value_type             value_type;
    typedef typename BaseType::difference_type        difference_type;
    typedef typename BaseType::pointer                pointer;
    typedef typename BaseType::reference              reference;
    typedef ContainerArrIter< ArrType, Container, START_IDX, END_IDX >   Self;
    typedef typename Container::iterator    SubIterator;

    ContainerArrIter() {}

    ContainerArrIter( ArrType &arr ) 
            : m_Arr(arr), m_Index(START_IDX)
            , m_SubIt( m_Arr[START_IDX].begin() )
            , m_EndIt( m_Arr[END_IDX-1].end() )
    {}

    ContainerArrIter( ArrType &arr, SubIterator subIt, int idx )
            : m_Arr(arr), m_SubIt(subIt), m_Index(idx)
            , m_EndIt( m_Arr[END_IDX-1].end() )
    {}

    bool operator== ( const Self &rhs ) const
    { return (m_SubIt == rhs.m_SubIt) && (m_Index == rhs.m_Index); }
    bool operator!= ( const Self &rhs ) const
    { return !(*this == rhs); }

    reference operator* () const
    { return *m_SubIt; }
    pointer operator-> () const
    { return &(operator*()); }

    Self& operator++ ()
    {
        // already in the end, or empty.
        if( m_SubIt == m_EndIt )
            throw std::out_of_range( "ContainerArrIter out_of_range!" );

        auto handleEnd = [&]() {
            while( m_Index < END_IDX-1 && m_SubIt == m_Arr[m_Index].end() )
                m_SubIt = m_Arr[++m_Index].begin();
        };

        if ( m_SubIt == m_Arr[m_Index].end() ) {
            handleEnd();
        } else if ( ++m_SubIt == m_Arr[m_Index].end() ) {
            handleEnd();
        } // if

        return *this;
    }

    Self& operator++ ( int )
    {
        Self ret = *this;
        ++(*this);
        return ret;
    }

/*
 *     Self& operator--()
 *     {
 *         // TODO
 *         return *this;
 *     }
 * 
 *     Self& operator--( int )
 *     {
 *         Self ret = *this;
 *         --(*this);
 *         return ret;
 *     }
 */

    ArrType&        m_Arr;
    SubIterator     m_SubIt;
    SubIterator     m_EndIt;
    int             m_Index;
};


class IntStringDB {
    static const int SIZE = 10;
    typedef std::map< int, std::string >    DBItem;
    typedef DBItem  DBType[SIZE];
public:
    typedef DBItem::value_type  value_type;
public:

private:
    DBType      m_db;
};


int main()
{
    using namespace std;

    // typedef vector<int>         VectorArr[10];
    // typedef list<int>           ListArr[10];

    // ContainerArrIter< vector<int>, 0, 10 > viter;    
    // ContainerArrIter< list<int>, 0, 10 > liter;    

    return 0;
}


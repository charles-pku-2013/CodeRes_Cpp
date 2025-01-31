#include <iostream>
#include <unistd.h>

using namespace std;

struct MoveDemo1 {
    MoveDemo1()
    { cout << "MoveDemo1 default constructor" << endl; }

    MoveDemo1( const MoveDemo1 &rhs )
    { cout << "MoveDemo1 copy constructor" << endl; }

    MoveDemo1( MoveDemo1 &&rhs )
    {
        cout << "MoveDemo1 move constructor" << endl;
        this->val = rhs.val;
    }

    MoveDemo1& operator= ( const MoveDemo1 &rhs )
    {
        cout << "MoveDemo1 copy assignment" << endl;
        return *this;
    }

    MoveDemo1& operator= ( MoveDemo1 &&rhs )
    {
        cout << "MoveDemo1 move assignment" << endl;
        return *this;
    }

    int val = 0;
};

MoveDemo1 func() {
    MoveDemo1 retval;
    retval.val = 5;
    ::sleep(3);
    return retval;
}

struct MoveDemo2 {
    MoveDemo2()
    { cout << "MoveDemo2 default constructor" << endl; }

    /*
     * MoveDemo1 move constructor   // arg
     * MoveDemo1 move constructor   // m_MoveDemo1
     */
    // MoveDemo2(MoveDemo1 arg) : m_MoveDemo1(std::move(arg))
    // { cout << "MoveDemo2 arg constructor" << endl; }

    /*
     * MoveDemo1 move constructor  // only one
     */
    // MoveDemo2(MoveDemo1&& arg) : m_MoveDemo1(std::move(arg))
    // { cout << "MoveDemo2 arg constructor" << endl; }

    /*
     * MoveDemo1 copy constructor
     */
    MoveDemo2(const MoveDemo1& arg) : m_MoveDemo1(arg)
    { cout << "MoveDemo2 arg constructor" << endl; }

/*
 *     MoveDemo2( const MoveDemo2 &rhs )
 *     { cout << "MoveDemo2 copy constructor" << endl; }
 *
 *     MoveDemo2( MoveDemo2 &&rhs )
 *     { cout << "MoveDemo2 move constructor" << endl; }
 *
 *     MoveDemo2& operator= ( const MoveDemo2 &rhs )
 *     {
 *         cout << "MoveDemo2 copy assignment" << endl;
 *         return *this;
 *     }
 *
 *     MoveDemo2& operator= ( MoveDemo2 &&rhs )
 *     {
 *         cout << "MoveDemo2 move assignment" << endl;
 *         return *this;
 *     }
 */

    MoveDemo1 getMember()
    { return m_MoveDemo1; }

    MoveDemo1&& getMoveMember()
    { return std::move(m_MoveDemo1); }

    MoveDemo1 m_MoveDemo1;
};


int main()
{
    // MoveDemo1 val = std::move(func());      // MoveDemo1 default constructor
                                            // MoveDemo1 move constructor
    MoveDemo1 val = func();  // MoveDemo1 default constructor  应该用这个
    // cout << val.val << endl;
    cin.get();
    MoveDemo2 md2(std::move(val));

    // MoveDemo2 md2;

    // MoveDemo1 md1 = md2.getMember();  // MoveDemo1 copy constructor

    //!! 由于原函数是返回值类型，这里先生成一个临时对象，然后再move
    /*
     * MoveDemo1 copy constructor
     * MoveDemo1 move constructor
     */
    // MoveDemo1 md1 = std::move(md2.getMember());

    //!! 原函数返回的就是引用，所以这里不会生成临时对象
    /*
     * MoveDemo1 move constructor
     */
    // MoveDemo1 md1 = std::move( md2.getMoveMember() );

    //!! 默认的 move constructor，会执行 member-wise move
    /*
     * MoveDemo1 move constructor
     */
    // MoveDemo2 md2_1 = std::move( md2 );
    // 就如同默认的 copy constructor 会执行 member-wise copy 一样
    /*
     * MoveDemo1 copy constructor
     */
    // MoveDemo2 md2_1 = md2;

    return 0;
}

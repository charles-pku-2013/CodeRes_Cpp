// AssignmentOperator.cpp : Defines the entry point for the console application.
//

// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#include "stdafx.h"
#include <string>

class CMyString
{
public:
    CMyString(char* pData = NULL);
    CMyString(const CMyString& str);
    ~CMyString(void);

    CMyString& operator = (const CMyString& str);

    void Print();
      
private:
    char* m_pData;
};

CMyString::CMyString(char *pData)
{
    if(pData == NULL)
    {
        m_pData = new char[1];
        m_pData[0] = '\0';
    }
    else
    {
        int length = strlen(pData);
        m_pData = new char[length + 1];
        strcpy(m_pData, pData);
    }
}

//!! 复制构造函数不必先删除原来的!!! 和赋值区别
CMyString::CMyString(const CMyString &str)
{
    int length = strlen(str.m_pData);
    m_pData = new char[length + 1];
    strcpy(m_pData, str.m_pData);
}

CMyString::~CMyString()
{
    delete[] m_pData;
}

/*
 * // Original version
 * CMyString& CMyString::operator = (const CMyString& str)
 * {
 *     if(this == &str)
 *         return *this;
 * 
 *     delete []m_pData;
 *     m_pData = NULL;
 * 
 *     m_pData = new char[strlen(str.m_pData) + 1];
 *     strcpy(m_pData, str.m_pData);
 * 
 *     return *this;
 * }
 */


// 方法1 传统方法+异常处理
CMyString& CMyString::operator = (const CMyString& str)
{
    // 1. 判断是否自我赋值
    if(this == &str)
        return *this;
    
    // 2. 尝试分配新内存
    char *new_pData = NULL;
    try {
        new_pData = new char[strlen(str.m_pData) + 1];
    } catch ( const std::bad_alloc &ex ) {
        cerr << ex.what() << endl;
        return *this;
    }

    strcpy(new_pData, str.m_pData);

    delete []m_pData;
    m_pData = new_pData;

    return *this;
}

// way2 swap 与临时变量交换
CMyString& CMyString::operator = (const CMyString& str)
{
    if( this != &str ) {
        CMyString tmp(str);     // make a copy of rvalue
        std::swap( m_pData, tmp.m_pData );
    } // if

    return *this;
}


// ====================测试代码====================
void CMyString::Print()
{
    printf("%s", m_pData);
}

void Test1()
{
    printf("Test1 begins:\n");

    char* text = "Hello world";

    CMyString str1(text);
    CMyString str2;
    str2 = str1;

    printf("The expected result is: %s.\n", text);

    printf("The actual result is: ");
    str2.Print();
    printf(".\n");
}

// 赋值给自己
void Test2()
{
    printf("Test2 begins:\n");

    char* text = "Hello world";

    CMyString str1(text);
    str1 = str1;

    printf("The expected result is: %s.\n", text);

    printf("The actual result is: ");
    str1.Print();
    printf(".\n");
}

// 连续赋值
void Test3()
{
    printf("Test3 begins:\n");

    char* text = "Hello world";

    CMyString str1(text);
    CMyString str2, str3;
    str3 = str2 = str1;

    printf("The expected result is: %s.\n", text);

    printf("The actual result is: ");
    str2.Print();
    printf(".\n");

    printf("The expected result is: %s.\n", text);

    printf("The actual result is: ");
    str3.Print();
    printf(".\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
    Test1();
    Test2();
    Test3();

    return 0;
}


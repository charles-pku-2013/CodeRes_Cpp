#include <iostream>
#include <cstring>
#include <utility> // 包含 std::swap

class String {
private:
    char* m_data;

public:
    // 1. 构造函数 (结合了默认构造函数)
    String(const char* str = nullptr) {
        if (str == nullptr) {
            m_data = new char[1];
            *m_data = '\0';
        } else {
            size_t length = strlen(str);
            m_data = new char[length + 1];
            strcpy(m_data, str);
        }
    }

    // 2. 拷贝构造函数 (深拷贝)
    String(const String& other) {
        size_t length = strlen(other.m_data);
        m_data = new char[length + 1];
        strcpy(m_data, other.m_data);
    }

    // 3. 拷贝赋值运算符 (使用 Copy-and-Swap 惯用法，异常安全且代码简洁)
    // 注意：这里的参数是传值 (String other)，会隐式调用拷贝构造函数创建临时对象
    String& operator=(String other) noexcept {
        std::swap(m_data, other.m_data); // 交换指针
        return *this; 
        // 离开作用域时，临时对象 other 被析构，顺便清理了原有的旧内存
    }

    /* 传统拷贝赋值运算符写法（供参考对比）：
    String& operator=(const String& other) {
        if (this != &other) { // 必须检查自我赋值！
            char* temp = new char[strlen(other.m_data) + 1]; // 先分配新内存，保证异常安全
            strcpy(temp, other.m_data);
            delete[] m_data; // 释放旧内存
            m_data = temp;
        }
        return *this;
    }
    */

    // 4. 移动构造函数 (C++11 核心考点：直接接管资源)
    String(String&& other) noexcept : m_data(other.m_data) {
        other.m_data = nullptr; // 将原对象的指针置空，防止析构时被 delete
    }

    // 5. 移动赋值运算符 (C++11)
    // 提示：如果使用了上面的 Copy-and-Swap，且参数传值，其实已经隐式包含了移动赋值的功能，
    // 但为了面试展示对移动语义的精确控制，通常还是会单独写出来。
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] m_data;       // 释放自己的内存
            m_data = other.m_data; // 接管对方的内存
            other.m_data = nullptr; // 将对方置空
        }
        return *this;
    }

    // 6. 析构函数
    ~String() {
        delete[] m_data; // 注意使用 delete[] 而不是 delete
    }

    // === 下方为方便测试的辅助函数 ===
    
    size_t size() const {
        return strlen(m_data);
    }

    const char* c_str() const {
        return m_data;
    }
};

// 测试代码
int main() {
    String s1("Hello");           // 调用普通构造
    String s2 = s1;               // 调用拷贝构造
    String s3;                    // 调用默认构造
    s3 = s2;                      // 调用拷贝赋值
    String s4 = std::move(s1);    // 调用移动构造
    s3 = std::move(s2);           // 调用移动赋值

    std::cout << "s4: " << s4.c_str() << std::endl; // 输出 Hello
    return 0;
}

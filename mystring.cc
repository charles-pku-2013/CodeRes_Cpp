#include <iostream>
#include <cstring>

class MyString {
public:
    MyString() = default;

    MyString(const char* s) {
        if (s) {
            std::size_t len = std::strlen(s);
            data_ = new char[len + 1];
            std::strcpy(data_, s);
            len_ = len;
        }
    }

    MyString(const MyString& rhs) {
        if (rhs.data_) {
            std::size_t len = rhs.len_;
            data_ = new char[len + 1];
            std::strcpy(data_, rhs.data_)
            len_ = len;
        }
    }

    MyString(MyString&& rhs) {
        if (rhs.data_) {
            data_ = rhs.data_;
            len_ = rhs.len_;
            rhs.data_ = nullptr;
            rhs.len_ = 0;
        }
    }

    MyString& operator=(const MyString& rhs) {
        if (this == &rhs)
        { return *this; }

        if (data_) {
            delete [] data_;
        }

        len_ = rhs.len_;
        data_ = new char[len_ + 1];
        std::strcpy(data_, rhs.data_);

        return *this;
    }

    MyString& operator=(MyString&& rhs) {
        if (this == &rhs)
        { return *this; }

        if (data_) {
            delete [] data_;
        }

        len_ = rhs.len_;
        data_ = rhs.data_;
        rhs.data_ = nullptr;
        rhs.len_ = 0;

        return *this;
    }

    ~MyString() {
        if (data_) {
            delete [] data_;
        }
    }

    std::size_t length() const
    { return len_; }

    const char* c_str() const
    { return data_; }

private:
    char* data_ = nullptr;
    std::size_t len_ = 0;
};


// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf

#include <streambuf>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>


class char_array_buffer : public std::streambuf
{
    public:
        char_array_buffer(const char *begin, const char *end);
        explicit char_array_buffer(const char *str);

    private:
        int_type underflow();
        int_type uflow();
        int_type pbackfail(int_type ch);
        std::streamsize showmanyc();

        // copy ctor and assignment not implemented;
        // copying not allowed
        char_array_buffer(const char_array_buffer &);
        char_array_buffer &operator= (const char_array_buffer &);

    private:
        const char * const begin_;
        const char * const end_;
        const char * current_;
};


char_array_buffer::char_array_buffer(const char *begin, const char *end) :
    begin_(begin),
    end_(end),
    current_(begin_)
{
    assert(std::less_equal<const char *>()(begin_, end_));
}

char_array_buffer::char_array_buffer(const char *str) :
    begin_(str),
    end_(begin_ + std::strlen(str)),
    current_(begin_)
{
}


char_array_buffer::int_type char_array_buffer::underflow()
{
    if (current_ == end_)
        return traits_type::eof();

    return traits_type::to_int_type(*current_);
}


char_array_buffer::int_type char_array_buffer::uflow()
{
    if (current_ == end_)
        return traits_type::eof();

    return traits_type::to_int_type(*current_++);
}


char_array_buffer::int_type char_array_buffer::pbackfail(int_type ch)
{
    if (current_ == begin_ || (ch != traits_type::eof() && ch != current_[-1]))
        return traits_type::eof();

    return traits_type::to_int_type(*--current_);
}


std::streamsize char_array_buffer::showmanyc()
{
    assert(std::less_equal<const char *>()(current_, end_));
    return end_ - current_;
}

















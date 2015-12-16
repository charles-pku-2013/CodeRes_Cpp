// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf

#include <streambuf>
#include <iosfwd>
#include <cstdlib>
#include <vector>

class caps_buffer : public std::streambuf
{
    public:
        explicit caps_buffer(std::ostream &sink, std::size_t buff_sz = 256);

    protected:
        bool do_caps_and_flush();

    private:
        int_type overflow(int_type ch);
        int sync();

        // copy ctor and assignment not implemented;
        // copying not allowed
        caps_buffer(const caps_buffer &);
        caps_buffer &operator= (const caps_buffer &);

    private:
        bool cap_next_;
        std::ostream &sink_;
        std::vector<char> buffer_;
};


caps_buffer::caps_buffer(std::ostream &sink, std::size_t buff_sz) :
    cap_next_(true),
    sink_(sink),
    buffer_(buff_sz + 1)
{
    sink_.clear();
    char *base = &buffer_.front();
    setp(base, base + buffer_.size() - 1); // -1 to make overflow() easier
}


caps_buffer::int_type caps_buffer::overflow(int_type ch)
{
    if (sink_ && ch != traits_type::eof())
    {
        assert(std::less_equal<char *>()(pptr(), epptr()));
        *pptr() = ch;
        pbump(1);
        if (do_caps_and_flush())
            return ch;
    }

    return traits_type::eof();
}


int caps_buffer::sync()
{
	return do_caps_and_flush() ? 0 : -1;
}


bool caps_buffer::do_caps_and_flush()
{
    for (char *p = pbase(), *e = pptr(); p != e; ++p)
    {
        if (*p == '.')
            cap_next_ = true;
        else if (std::isalpha(*p))
        {
            if (cap_next_)
                *p = std::toupper(*p);

            cap_next_ = false;
        }
    }
    std::ptrdiff_t n = pptr() - pbase();
    pbump(-n);

    return sink_.write(pbase(), n);
}



















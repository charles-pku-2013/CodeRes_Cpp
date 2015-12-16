// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf

#include <streambuf>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>


class FILE_buffer : public std::streambuf
{
    public:
        explicit FILE_buffer(FILE *fptr, std::size_t buff_sz = 256, std::size_t put_back = 8);

    private:
        // overrides base class underflow()
        int_type underflow();

        // copy ctor and assignment not implemented;
        // copying not allowed
        FILE_buffer(const FILE_buffer &);
        FILE_buffer &operator= (const FILE_buffer &);

    private:
        FILE *fptr_;
        const std::size_t put_back_;
        std::vector<char> buffer_;
};


using std::size_t;

FILE_buffer::FILE_buffer(FILE *fptr, size_t buff_sz, size_t put_back) :
    fptr_(fptr),
    put_back_(std::max(put_back, size_t(1))),
    buffer_(std::max(buff_sz, put_back_) + put_back_)
{
    char *end = &buffer_.front() + buffer_.size();
    setg(end, end, end); // repositions the beginning, next, and end pointers of the input sequence 
}


std::streambuf::int_type FILE_buffer::underflow()
{
    if (gptr() < egptr()) // buffer not exhausted
        return traits_type::to_int_type(*gptr());

    char *base = &buffer_.front();
    char *start = base;

    if (eback() == base) // true when this isn't the first fill
    {
        // Make arrangements for putback characters
        std::memmove(base, egptr() - put_back_, put_back_);
        start += put_back_;
    }

    // start is now the start of the buffer, proper.
    // Read from fptr_ in to the provided buffer
    size_t n = std::fread(start, 1, buffer_.size() - (start - base), fptr_);
    if (n == 0)
        return traits_type::eof();

    // Set buffer pointers
    setg(base, start, start + n);

    return traits_type::to_int_type(*gptr());
}















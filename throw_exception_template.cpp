
inline
void throw_runtime_error( const std::ostream &os )
{
    using namespace std;
    const stringstream &_str = dynamic_cast<const stringstream&>(os);
    throw runtime_error(_str.str());
}

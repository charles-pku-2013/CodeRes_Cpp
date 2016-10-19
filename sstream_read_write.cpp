#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

inline 
std::size_t Filesize(std::istream &fin)
{
	std::size_t pos = fin.tellg();
	fin.seekg(0, std::ios_base::end);
	std::size_t sz = fin.tellg();
	fin.seekg(pos);
	return sz;
}

void read_file(const std::string &in, std::string &content)
{
    ifstream ifs(in, ios::in | ios::binary);
    auto sz = Filesize(ifs);
    vector<char> buf(sz);
    ifs.read(&buf[0], sz);

    ostringstream stream(ios::out | ios::binary);
    stream.write(&buf[0], sz);
    stream.flush();

    content = stream.str();
}

int main()
{
    string content;
    read_file("/tmp/in.bin", content);
    cout << content << endl;
    cout << content.length() << endl;

    return 0;
}



inline 
std::size_t Filesize(std::istream &fin)
{
	std::size_t pos = fin.tellg();
	fin.seekg(0, std::ios_base::end);
	std::size_t sz = fin.tellg();
	fin.seekg(pos);
	return sz;
}


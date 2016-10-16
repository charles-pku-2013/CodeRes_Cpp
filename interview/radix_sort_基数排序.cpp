#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <iterator>
#include <thread>
#include <memory>


using namespace std;

template <typename T>
struct Bucket {
	Bucket()
	{
		m_bucket.resize(10);
	}

	void push(int idx, const T &val)
	{
		m_bucket[idx].push_back(val);
	}

	template <typename Iter>
	void dumpClear(Iter outIt)
	{
		for (int i = 0; i < 10; ++i) {
			std::copy(m_bucket[i].begin(), m_bucket[i].end(), outIt);
			outIt += m_bucket[i].size();	//!! NOTE!!!
			m_bucket[i].clear();
		} // for i
	}

	std::vector<std::list<T>>	m_bucket;
};


template <typename Iter>
void RadixSort(Iter beg, Iter end)
{
	typedef typename Iter::value_type	value_type;

	if (beg >= end - 1) return;

	int base = 1;
	bool cont = false;
	Bucket<value_type> bucket;

	do {
		cont = false;
		for (Iter i = beg; i != end; ++i) {
			value_type val = (*i) / base;
			if (val) cont = true;
			int idx = val % 10;
			bucket.push(idx, *i);
		} // for i
		bucket.dumpClear(beg);
		base *= 10;
	} while (cont);
}




void test1()
{
	vector<int> a{ 1, 3, 5, 7, 9 };
}

void test2()
{
	vector<int> a{ 170, 45, 75, 90, 802, 2, 24, 66 };
	RadixSort(a.begin(), a.end());
	copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}


int main(int argc, char **argv)
{
	try {
		//test1();
		test2();
	} catch (const std::exception &ex) {
		cerr << "Exception caught by main: " << ex.what() << endl;
		return -1;
	}

    return 0;
}




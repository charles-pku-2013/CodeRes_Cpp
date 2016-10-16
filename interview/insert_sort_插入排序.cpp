#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <thread>
#include <memory>


using namespace std;

// 返回插入位置
template <typename Iter>
Iter BinarySearch(Iter beg, Iter end, const typename Iter::value_type &val)
{
	while (beg < end) {
		Iter mid = beg + std::distance(beg, end) / 2;
		if (val == *mid) return mid;
		if (val < *mid) end = mid;
		else beg = mid + 1;
	} // while

	return beg;
}

template <typename Iter>
void InsertSort(Iter beg, Iter end)
{
	typedef typename Iter::value_type	value_type;

	if (beg >= end || beg + 1 == end) return;

	for (Iter i = beg + 1; i != end; ++i) {
		value_type val = *i;
		Iter pos = BinarySearch(beg, i, val);
		std::copy(pos, i, pos + 1);
		*pos = val;
	} // for i
}

void test1()
{
	vector<int> a{ 1, 3, 5, 7, 9 };
	auto it = BinarySearch(a.begin(), a.end(), 6);
	cout << *it << endl;
}

void test2()
{
	vector<int> a{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
	InsertSort(a.begin(), a.end());
	copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}


int main(int argc, char **argv)
{
	try {
		//test1();
		test2();
	}
	catch (const std::exception &ex) {
		cerr << "Exception caught by main: " << ex.what() << endl;
		return -1;
	}

    return 0;
}




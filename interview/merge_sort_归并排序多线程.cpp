#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <thread>


using namespace std;

template <typename Iter, typename OutIter>
void merge_sorted(Iter beg1, Iter end1, Iter beg2, Iter end2, OutIter outIt)
{
	while (beg1 != end1 && beg2 != end2) {
		if (*beg1 < *beg2)
			*outIt++ = *beg1++;
		else
			*outIt++ = *beg2++;
	} // while

	while (beg1 != end1)
		*outIt++ = *beg1++;
	while (beg2 != end2)
		*outIt++ = *beg2++;
}


template <typename Iter>
void merge_sort(Iter beg, Iter end)
{
	typedef typename Iter::value_type value_type;

	if (beg >= end || beg + 1 == end)
		return;

	std::size_t n = std::distance(beg, end);
	if (n == 2) {
		if (*beg > *(beg + 1))
			std::iter_swap(beg, beg + 1);
		return;
	} // if

	Iter mid = beg + n / 2;
	merge_sort(beg, mid);
	merge_sort(mid, end);

	std::vector<value_type> tmp(n);
	merge_sorted(beg, mid, mid, end, tmp.begin());
	std::copy(tmp.begin(), tmp.end(), beg);
}


template <typename Iter>
void merge_sort_mt(Iter beg, Iter end)
{
	typedef typename Iter::value_type value_type;

	if (beg >= end || beg + 1 == end)
		return;

	std::size_t n = std::distance(beg, end);
	if (n == 2) {
		if (*beg > *(beg + 1))
			std::iter_swap(beg, beg + 1);
		return;
	} // if

	Iter mid = beg + n / 2;
	std::thread t1(std::bind(merge_sort<Iter>, beg, mid));
	std::thread t2(std::bind(merge_sort<Iter>, mid, end));

	t1.join(); t2.join();

	std::vector<value_type> tmp(n);
	merge_sorted(beg, mid, mid, end, tmp.begin());
	std::copy(tmp.begin(), tmp.end(), beg);
}


int main(int argc, char **argv)
{
	try {
		std::vector<int> v{ 5, 6, 4, 3, 2, 6, 7, 9, 3 };
		merge_sort_mt(v.begin(), v.end());
		copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
		cout << endl;

	} catch (const std::exception &ex) {
		cerr << "Exception caught by main: " << ex.what() << endl;
		return -1;
	}

    return 0;
}


void test1()
{
	vector<int> arr{ 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
	vector<int> out;
	auto mid = arr.begin() + arr.size() / 2;
	merge_sorted(arr.begin(), mid, mid, arr.end(), back_inserter(out));
	copy(out.begin(), out.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}

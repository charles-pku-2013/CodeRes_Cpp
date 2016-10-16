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

template <typename Iter>
std::ostream& print_range(std::ostream &os, Iter beg, Iter end)
{
	std::copy(beg, end, std::ostream_iterator<typename Iter::value_type>(os, " "));
	os << endl;
	return os;
}


// bigger root heap
//!! NOTE!!! 这种调整只适用于下标从1开始
template <typename T>
void AdjustHeap(T *a, int s, int n)
{
	typename T save = a[s];
	for (int j = 2 * s; j <= n; j *= 2) {
		if (j + 1 <= n && a[j] < a[j + 1]) ++j;
		if (save >= a[j]) break; //!! NOTE!!!	不是 a[s] >= a[j]
		a[s] = a[j]; s = j;
	} // for
	a[s] = save;
}

//!! 下标从1开始，末尾包括n
template <typename T>
void MakeHeap(T *a, int n)
{
	for (int i = n / 2; i >= 1; --i)
		AdjustHeap(a, i, n);
}

template <typename Iter>
void HeapSort(Iter beg, Iter end)
{
	if (beg >= end - 1) return;

	typedef typename Iter::pointer pointer;
	int n = (int)std::distance(beg, end);
	pointer a = &(*beg); --a; //!! 为了使下标从1开始

	MakeHeap(a, (int)n);

	while (n > 1) {
		std::swap(a[1], a[n]);
		AdjustHeap(a, 1, --n);
	} // for
}


void test1()
{
	vector<int> a{ 1, 3, 5, 7, 9 };
}

void test2()
{
	vector<int> a{ 49, 38, 65, 97, 76, 13, 27, 49 };
	HeapSort(a.begin(), a.end());
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




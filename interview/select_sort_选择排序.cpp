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

template <typename Iter>
void SelectSort(Iter beg, Iter end)
{
	if (beg >= end || beg + 1 == end) return;

	for (; beg != end - 1; ++beg) {
		Iter minIt = beg;
		for (Iter j = beg + 1; j != end; ++j) {
			if (*j < *minIt)
				minIt = j;
		} // for j
		std::iter_swap(beg, minIt);
	} // for
}


void test1()
{
	vector<int> a{ 1, 3, 5, 7, 9 };
}

void test2()
{
	vector<int> a{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
	SelectSort(a.begin(), a.end());
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




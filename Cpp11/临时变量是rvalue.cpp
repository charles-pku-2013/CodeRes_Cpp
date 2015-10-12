#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

void func( vector<int> &v )
{ 
	copy( v.begin(), v.end(), ostream_iterator<int>(cout, " ") );
	cout << endl;
}


int main()
{
	func( vector<int>() );	//!! 临时变量是rvalue
	
	return 0;
}




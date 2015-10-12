// http://acm.hdu.edu.cn/showproblem.php?pid=2602

#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

#define UP 1
#define UPLEFT 2

template< typename T >
void printArray( const T &a )
{
    for( typename T::const_iterator it = a.begin(); it != a.end(); ++it )
        cout << *it << " ";
    cout << endl;
}

// values weights num capacity
int knapsack(int *v, int *w, int n, int c)
{
	vector<int> dp(c+1, 0);
	for( int i=0; i<n; ++i ) {
        for( int j=c; j>=w[i]; --j ) {
            if( dp[j] < dp[ j-w[i] ]+v[i] )     // 判断是否放入第i个物品
                dp[j]=dp[ j-w[i] ]+v[i];
        } // for
        cout << "dp array: ";
        printArray(dp);
    } // for
	return dp[c];
}



int main()
{
    int nCases, n, capacity;
    
    scanf( "%d\n", &nCases );
    while( nCases-- ) {
        scanf( "%d %d\n", &n, &capacity );
        vector<int> weights(n, 0);
        vector<int> values(n, 0);
        for( int i = 0; i < n; ++i )
            scanf( "%d", &weights[i] );
        for( int i = 0; i < n; ++i )
            scanf( "%d", &values[i] );
        
//        printArray( weights );
//        printArray( values );
        int ret = knapsack( &values[0], &weights[0], n, capacity );
        printf("%d\n", ret);
    }
    
	return 0;
}


















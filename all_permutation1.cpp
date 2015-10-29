#include <cstdio>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

//!! 回朔法

/* Function to print permutations of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string. */
void permute(char *a, int l, int r)
{
    int i;
    if (l == r) {
        printf("%s\n", a);
        return;
    }

    for (i = l; i <= r; i++) {
        std::swap(a[l], a[i]);
        permute(a, l+1, r);
        std::swap(a[l], a[i]);
    } // for 
}
 
/* Driver program to test above functions */
int main()
{
    char str[] = "ABC";
    int n = strlen(str);
    permute(str, 0, n-1);

    return 0;
}

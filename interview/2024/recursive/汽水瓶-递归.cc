/*
https://www.nowcoder.com/practice/fe298c55694f4ed39e256170ff2c205f?tpId=37&tqId=21245&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D3%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
using namespace std;

int caculate(int n) {
    if (n < 2) { return 0; }
    if (n == 2 || n == 3) { return 1; }
    int k = n / 3;
    int m = n % 3;
    return k + caculate(k + m);
}

int main() {
    int n;
    while (cin >> n) {
        if (n == 0) { break; }
        int result = caculate(n);
        cout << result << endl;
    }
}
// 64 位输出请用 printf("%lld")

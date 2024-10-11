#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

void test_includes() {
    vector<int> a{1,2,3};
    vector<int> b{1,2,3,4};
    cout << std::includes(a.begin(), a.end(), b.begin(), b.end()) << endl;  // 0
    cout << std::includes(b.begin(), b.end(), a.begin(), a.end()) << endl;  // 1  第一对iter是父，第二对iter是子

    string s("hello world");
    string t("WOR");
    cout << std::includes(s.begin(), s.end(), t.begin(), t.end(), [](char a, char b)->bool {
        return std::tolower(a) < std::tolower(b);
    }) << endl;  // 1
}

void test_set_union() {
    vector<int> a{1,3,5,7};
    vector<int> b{2,4,6,8};
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;
}

void test_set_intersection() {
    vector<int> a{1,3,5,7};
    vector<int> b{0,1,3,8};
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;
}

void test_set_difference() {
    vector<int> a{1,3,5,7};
    vector<int> b{0,1,3,8};
    // II中出现过的I中元素从I中删除
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;  // 5 7
}

void test_set_symmetric_difference() {
    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> v2{5, 7, 9, 10};
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;
    // 去掉交集
    std::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;  // 1 2 3 4 6 8 9 10
    std::set_symmetric_difference(v2.begin(), v2.end(), v1.begin(), v1.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;  // same as above
}

void test_merge() {
    vector<int> a{1,3,5,7};
    vector<int> b{0,1,3,8};
    std::merge(a.begin(), a.end(), b.begin(), b.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;  // 0 1 1 3 3 5 7 8
}

void test_inplace_merge() {
    vector<int> a{1,3,5,2,4,6};
    auto it = std::partition(a.begin(), a.end(), [](int v)->bool {
        return v % 2 == 1;
    });  // 把前半部分和后半部分合并，前后必须事先有序
    cout << *it << endl;
    std::inplace_merge(a.begin(), it, a.end());
    std::copy(a.begin(), a.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;
}

int main() {
    // test_includes();
    // test_set_union();
    // test_set_intersection();
    // test_set_difference();
    // test_set_symmetric_difference();
    // test_merge();
    test_inplace_merge();

    return 0;
}


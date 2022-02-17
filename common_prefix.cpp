#include <iostream>
#include <vector>
#include <iterator>

template<typename Sequence1, typename Sequence2, typename OutSequence>
void common_prefix(Sequence1 beg1, Sequence1 end1,
                   Sequence2 beg2, Sequence2 end2, OutSequence outIt) {
    for(; beg1 != end1 && beg2 != end2 && *beg1 == *beg2; ++beg1, ++beg2) {
        *outIt++ = *beg1;
    }
}

int main() {
    using namespace std;

    string s1("/Users/sunchao11/Documents/Devel/CodeRes/CPP");
    string s2("/Users/sunchao11/Documents/Devel/jd/");

    string out;
    common_prefix(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(out));
    cout << out << endl;

    return 0;
}


#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

typedef std::vector< std::vector<std::string> >     StringMatrix;

void print_all(const StringMatrix &mat, size_t idx, std::vector<std::size_t> &work )
{
    if (idx >= mat.size()) {
        for (size_t i = 0; i != work.size(); ++i) {
            size_t j = work[i];
            cout << mat[i][j] << " ";
        } // for
        // copy(work.begin(), work.end(), ostream_iterator<string>(cout, " "));
        cout << endl;
        return;
    } // if

    const vector<string> &arr = mat[idx];
    for (size_t i = 0; i != arr.size(); ++i) {
        work[idx] = i;
        print_all(mat, idx + 1, work);
    } // for
}

int main()
{
    StringMatrix    mat;
    
    {
        vector<string> arr{"李宇春"};
        mat.emplace_back();
        mat.back().swap(arr);
    }

    {
        vector<string> arr{"居然", "真没想到", "怪不得"};
        mat.emplace_back();
        mat.back().swap(arr);
    }

    {
        vector<string> arr{"结婚", "订婚", "抢婚", "结了婚"};
        mat.emplace_back();
        mat.back().swap(arr);
    }

    {
        vector<string> arr{"了", "后来", "说", "以后"};
        mat.emplace_back();
        mat.back().swap(arr);
    }

    vector<size_t> tmp(mat.size(), 0);
    print_all(mat, 0, tmp);

    return 0;
}


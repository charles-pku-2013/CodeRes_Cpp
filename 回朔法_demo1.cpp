#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <deque>
#include <cmath>
#include <cassert>
// #include "LOG.h"
// http://www.lintcode.com/zh-cn/problem/word-search/

using namespace std; 

class Solution {
public:
    void ResetMark( const vector< vector<char> > &board, 
                        vector< vector<bool> > &mark )
    {
        mark.clear();
        mark.resize( board.size() );
        for( size_t i = 0; i < board.size(); ++i )
            mark[i].resize( board[i].size(), false );
    }

    void Search( const vector< vector<char> > &board, 
                        const string &word, size_t row, size_t col, size_t index,
                        vector< vector<bool> > &mark )
    {
        if( found ) return;         //!! 搜索剪枝

        if( index == word.length()-1 ) {
            found = true;
            return;
        } // if 

        mark[row][col] = true;

        size_t nROW = board.size();
        size_t len = board[row].size();

        // left
        if( col > 0 && !mark[row][col-1] && board[row][col-1] == word[index+1] )
            Search( board, word, row, col-1, index+1, mark );
        // up 
        if( row > 0 && !mark[row-1][col] && board[row-1][col] == word[index+1] )
            Search( board, word, row-1, col, index+1, mark );
        // right 
        if( col < len-1 && !mark[row][col+1] && board[row][col+1] == word[index+1] )
            Search( board, word, row, col+1, index+1, mark );
        // down
        if( row < nROW-1 && !mark[row+1][col] && board[row+1][col] == word[index+1] )
            Search( board, word, row+1, col, index+1, mark );

        mark[row][col] = false;
    }


    bool exist(vector<vector<char> > &board, string word) 
    {
        const size_t ROW = board.size();
        vector< vector<bool> > mark;

        found = false;
        for( size_t i = 0; i < ROW; ++i ) {
            size_t lenCol = board[i].size();
            for( size_t j = 0; j < lenCol; ++j ) {
                if ( board[i][j] == word[0] ) {
                    ResetMark( board, mark );
                    Search( board, word, i, j, 0, mark );
                    if( found ) return true;
                } // if
            } // for 
        } // for 

        return false;
    }

private:
    bool found;
};

int main( int argc, char **argv )
{
    Solution sln;

    int n_row, n_test; 
    string str;

    cin >> n_row >> n_test;
    vector< vector<char> > board(n_row);
    for( int i = 0; i < n_row; ++i ) {
        cin >> str;
        board[i].assign( str.begin(), str.end() );
        // copy( board[i].begin(), board[i].end(), ostream_iterator<char>(cout, "") );
        // cout << endl;
    } // for 

    while( n_test-- ) {
        cin >> str;
        // cout << str << endl;
        bool ret = sln.exist( board, str );
        string result = ( ret ? "true" : "false" );
        cout << result << endl;
    } // while 

    return 0;
}

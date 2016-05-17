/*
 * c++ -o /tmp/test variadic_template_demo.cpp -lglog -std=c++11 -g
 * GLOG_logtostderr=1 /tmp/test
 */
/*
 * This file contains code from "C++ Primer, Fifth Edition", by Stanley B.
 * Lippman, Josee Lajoie, and Barbara E. Moo, and is covered under the
 * copyright and warranty notices given in that book:
 * 
 * "Copyright (c) 2013 by Objectwrite, Inc., Josee Lajoie, and Barbara E. Moo."
 * 
 * 
 * "The authors and publisher have taken care in the preparation of this book,
 * but make no expressed or implied warranty of any kind and assume no
 * responsibility for errors or omissions. No liability is assumed for
 * incidental or consequential damages in connection with or arising out of the
 * use of the information or programs contained herein."
 * 
 * Permission is granted for this code to be used for educational purposes in
 * association with the book, given proper citation if and when posted or
 * reproduced.Any commercial use of this code requires the explicit written
 * permission of the publisher, Addison-Wesley Professional, a division of
 * Pearson Education, Inc. Send your request for permission, stating clearly
 * what code you would like to use, and in what specific way, to the following
 * address: 
 * 
 *  Pearson Education, Inc.
 *  Rights and Permissions Department
 *  One Lake Street
 *  Upper Saddle River, NJ  07458
 *  Fax: (201) 236-3290
*/ 

#include <glog/logging.h>
#include <iostream>
using std::cerr;
using std::ostream; using std::cout; using std::endl;

#include <string>
using std::string;

#include <map>
using std::map;

#include <cstddef>
using std::size_t;

#include <utility>
using std::is_same;


// function to end the recursion and print the last element
template<typename T>
void print(const T &t)  // for the last arg in rest
{
    LOG(INFO) << "print single value version...";

    // std::is_same 判断类型是否一样
    // if( is_same< T, int>() ) {
        // cerr << "print int" << endl;
    // } else if( is_same< T, char>() ) {
        // cerr << "print char" << endl;
    // } else if( is_same< T, char*>() ) {
        // cerr << "print string" << endl;
    // } else {
        // cerr << "print unknown" << endl;
    // } // if
    cerr << t; // no separator after the last element in the pack
}

template <typename T, typename... Args>                 
void print(const T &t, const Args&... rest)//expand Args
{
    LOG(INFO) << "print variadic version...";
    cerr << t << ", ";              //!! for the first arg
    print(rest...);                     //expand rest
}



int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);

    print(10, "Hello", 'a', 12);
    cerr << endl;
    
    return 0;
}







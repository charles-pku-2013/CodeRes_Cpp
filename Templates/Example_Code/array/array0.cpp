/* simple example for using class array<>
 *
 * (C) Copyright Nicolai M. Josuttis 2001.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <boost/array.hpp>
#include <iostream>

int main()
{
    boost::array<int,10> a;
    boost::array<bool,0> b;

    // access elements via index (which should do nothing)
    for (unsigned i=1; i<b.size(); ++i) {
	std::cout << "oops" << std::endl;
    }

    // access elements via iterator (which should do nothing)
    boost::array<bool,0>::const_iterator pos;
    for (pos=b.begin(); pos!=b.end(); ++pos) {
	std::cout << "oops" << std::endl;
    }
}


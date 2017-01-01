/*
 * http://en.cppreference.com/w/cpp/language/operators
 */

T& operator*() const
{ auto p = check(curr, "dereference past end"); 
  return (*p)[curr];  // (*p) is the vector to which this object points
}
T* operator->() const
{ // delegate the real work to the dereference operator
 return & this->operator*(); 
}
T* operator&()
{ return pointer; }


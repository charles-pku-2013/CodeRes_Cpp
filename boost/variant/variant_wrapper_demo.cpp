#include <boost/variant.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

struct Add;
struct Sub;

// Add Sub 都包含 Expr 本身，所以用 recursive_wrapper
typedef boost::variant<int, boost::recursive_wrapper<Add>, 
                boost::recursive_wrapper<Sub> > Expr;

// another example from DeepDetect
// typedef mapbox::util::variant<std::string,double,int,bool,
        // std::vector<std::string>,std::vector<double>,std::vector<int>,std::vector<bool>,
        // mapbox::util::recursive_wrapper<std::vector<APIData>>> ad_variant_type;


struct Sub
{
   Sub() { }
   Sub(const Expr& l, const Expr& r) : lhs_(l), rhs_(r) { }
   Sub(const Sub& other) : lhs_(other.lhs_), rhs_(other.rhs_) { }

   Expr lhs_;
   Expr rhs_;
};

struct Add
{
   Add() { }
   Add(const Expr& l, const Expr& r) : lhs_(l), rhs_(r) { }
   Add(const Add& other) : lhs_(other.lhs_), rhs_(other.rhs_) { }

   Expr lhs_;
   Expr rhs_;
};

std::ostream& operator<<(std::ostream& out, const Add& a)
{
   out << '(' << a.lhs_ << '+' << a.rhs_ << ')';
   return out;
}

std::ostream& operator<<(std::ostream& out, const Sub& a)
{
   out << '(' << a.lhs_ << '-' << a.rhs_ << ')';
   return out;
}

//
// Expression evaluation visitor
//
struct Calculator : boost::static_visitor<int> // 模板参数指定返回类型
{
   Calculator()  { }

   int operator()(Add& x) const
   {
      Calculator calc;
      int n1 = boost::apply_visitor(calc, x.lhs_);
      int n2 = boost::apply_visitor(calc, x.rhs_);
      
      return n1 + n2;
   }

   int operator()(Sub& x) const
   {
      return boost::apply_visitor(Calculator(), x.lhs_) 
         - boost::apply_visitor(Calculator(), x.rhs_);
   }

   int operator()(Expr& x) const // 递归调用，直到x的值是int
   {
      Calculator calc;
      return boost::apply_visitor(calc, x);
   }

   int operator()(int x) const
   { return x; }

}; // Calculator


int main()
{
    int n = 13;
    Expr e1( Add(n, Sub(Add(40,2),Add(10,4))) ); //n + (40+2)-(10+14) = n+28
    cout << e1 << endl;

    int res = boost::apply_visitor(Calculator(), e1);
    cout << res << endl;

    return 0;
}

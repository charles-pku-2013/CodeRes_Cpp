#include <cstdio>

struct Base {
	virtual void test()
	{ printf("Base::test()\n"); }
};


struct Derived1 : Base {
	void test()
	{ printf("D1::test()\n"); }
};


struct Derived2 : Derived1 {
	void test()
	{ printf("D2::test()\n"); }
};


struct Derived3 : Derived2 {
	void test()
	{ printf("D3::test()\n"); }
};


int main()
{
	Derived2 *p = new Derived3;
	p->test();
	return 0;
}


















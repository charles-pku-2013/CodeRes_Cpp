#include <cstdio>
#include <iostream>

//!! ... 可变参数只能在最后，如同c++里默认参数
//!! __VA_ARGS__ 展开包括逗号,
#define PRINT_MACRO(arg1, arg2, ...) printf("%s\n%s\n%s\n", #arg1, #arg2, #__VA_ARGS__)

int main(int argc, char **argv)
{
	int i = 5;
	
	//!! 这里不检查变量 j
	PRINT_MACRO( i * 3, std::cout << i, i + 1, i + 2, j + 3 );
	
	return 0;
}


/* 
i * 3
std::cout << i
i + 1, i + 2, j + 3
 */




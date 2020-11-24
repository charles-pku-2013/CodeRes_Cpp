#include <iostream>
#include <memory>
#include <cstdio>

using namespace std;

//!! lambda 不能用一般的函数指针类型来表示
// typedef int (*DelType)( FILE* ); 在此例中可以和 fclose 结合
// using DelType = int( FILE* ); 这样是不可以的
// invalid initialization of reference of type ‘int (&)(_IO_FILE*)’ from expression of type ‘int (*)(_IO_FILE*)’
int main()
{	
	auto del = []( FILE *fp ) {
					cout << "customized FILE closer" << endl;
					fclose(fp);
				};
				
	unique_ptr< FILE, decltype(del)> pFILE( 
					fopen("/tmp/test.txt", "w"),
					del
				);
				
	fprintf( pFILE.get(), "This is a test." );
	
	getchar();
	
	return 0;
}
















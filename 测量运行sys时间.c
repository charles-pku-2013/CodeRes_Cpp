#include <stdio.h>
#include <time.h>
#include <unistd.h>

// 这种方法测到的是sys time，比实际的要长，尤其多线程，应该使用times函数
int main()
{
    clock_t start, finish;
    unsigned int i = 0;
    double duration;
    
    start = clock();
    while( ++i );
    finish = clock();
    
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("duration = %lf seconds\n", duration);
    
    return 0;
}















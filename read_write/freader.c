#include <unistd.h>
#include <stdio.h>

#define BUFSIZE     4

int main()
{
    char buf[BUFSIZE];

    /* setvbuf( stdin, NULL, _IONBF, 0 ); */

    size_t n;
    while( (n = fread(buf, 1, BUFSIZE, stdin)) > 0 ) {
        printf("read %zu bytes: ", n);
        for( size_t i = 0; i < n; ++i )
            printf("%02u", (unsigned char)buf[i]);
        putchar('\n');
        /* sleep(1); */
        /* usleep(10000);  */
    } // while

    return 0;
}


// ******* fast writer, slow reader *********
// 和直接用fd效果一样, OS大概有64K缓冲区
// 当OS缓冲区满时，fwrite调用会阻塞，直到缓冲区有空位, 不会丢包
/*
 * write 16376
 * write 16377
 * write 16378
 * write 16379
 * write 16380
 * write 16381
 * write 16382
 * write 16383
 * write 16384
 * write 16385
 * read 4 bytes: 02000000
 * write 16386
 * read 4 bytes: 03000000
 * write 16387
 * read 4 bytes: 04000000
 * write 16388
 * read 4 bytes: 05000000
 * write 16389
 * read 4 bytes: 06000000
 * write 16390
 */


// 当没有setvbuf _IONBUF 时候
// 实验结论：当OS缓冲区满时，会整个memcpy到libc缓冲区，然后腾空OS缓冲区，writer变得可写。
/*
 * read 4 bytes: ... 
 * ...
 * write 24562
 * write 24563
 * write 24564
 * write 24565
 * write 24566
 * write 24567
 * write 24568
 * write 24569
 * write 24570
 * write 24571
 * write 24572
 * write 24573
 * write 24574
 * write 24575
 * write 24576
 * read 4 bytes: 02000000
 * read 4 bytes: 03000000
 * read 4 bytes: 04000000
 * read 4 bytes: 05000000
 * read 4 bytes: 06000000
 * read 4 bytes: 07000000
 * read 4 bytes: 08000000
 */



// *********** fast reader, slow writer ************
// 未设置_IONBUF, reader 等 slow writer 情况
//!! 特别要注意这种情况下能否及时获取数据
// 等OS缓冲区满了，copy到libc缓冲区，然后从libc缓冲区开始读
/*
 * write 4090
 * write 4091
 * write 4092
 * write 4093
 * write 4094
 * write 4095
 * write 4096
 * write 4097
 * read 4 bytes: 01000000
 * read 4 bytes: 02000000
 * read 4 bytes: 03000000
 * read 4 bytes: 04000000
 * read 4 bytes: 05000000
 * read 4 bytes: 06000000
 * read 4 bytes: 07000000
 * read 4 bytes: 08000000
 * read 4 bytes: 09000000
 * read 4 bytes: 10000000
 */

// 当设置了 _IONBUF
/*
 * write 1
 * read 4 bytes: 01000000
 * write 2
 * read 4 bytes: 02000000
 * write 3
 * read 4 bytes: 03000000
 * write 4
 * read 4 bytes: 04000000
 * write 5
 * read 4 bytes: 05000000
 * write 6
 * read 4 bytes: 06000000
 * write 7
 * read 4 bytes: 07000000
 * write 8
 * read 4 bytes: 08000000
 */

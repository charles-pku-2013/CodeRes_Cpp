// 演示 read 实际读入字节与期望不一样


#include <unistd.h>
#include <stdio.h>

/*
 * #define BUFSIZE     1024
 * 
 * int main()
 * {
 *     char buf[BUFSIZE];
 * 
 *     ssize_t n;
 *     while( (n = read(STDIN_FILENO, buf, BUFSIZE)) > 0 ) {
 *         printf("read %zd bytes: ", n);
 *         for( ssize_t i = 0; i < n; ++i )
 *             printf("%02u", (unsigned char)buf[i]);
 *         putchar('\n');
 *         sleep(10); //!! 只要缓冲区不满，writer 不会因为 reader 的睡眠而阻塞，会一直向缓冲区写，直到满
 *     } // while
 * 
 *     return 0;
 * }
 */

// 使用了OS提供的缓存
/*
 * write 1
 * read 4 bytes: 01000000
 * write 2
 * write 3
 * write 4
 * write 5
 * write 6
 * write 7
 * write 8
 * write 9
 * write 10
 * read 36 bytes: 020000000300000004000000050000000600000007000000080000000900000010000000
 * write 11
 * write 12
 * write 13
 * write 14
 * write 15
 * write 16
 * write 17
 * write 18
 * write 19
 * write 20
 * read 40 bytes: 11000000120000001300000014000000150000001600000017000000180000001900000020000000
 */



#define BUFSIZE     4

int main()
{
    char buf[BUFSIZE];

    ssize_t n;
    while( (n = read(STDIN_FILENO, buf, BUFSIZE)) > 0 ) {
        printf("read %zd bytes: ", n);
        for( ssize_t i = 0; i < n; ++i )
            printf("%02u", (unsigned char)buf[i]);
        putchar('\n');
        sleep(1); //!! 只要缓冲区不满，writer 不会因为 reader 的睡眠而阻塞，会一直向缓冲区写，直到满
    } // while

    return 0;
}


// 当OS缓冲区满时，write调用会阻塞，直到缓冲区有空位
/*
 * write 16373
 * write 16374
 * write 16375
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
 * read 4 bytes: 07000000
 * write 16391
 * read 4 bytes: 08000000
 * write 16392
 * read 4 bytes: 09000000
 * write 16393
 * read 4 bytes: 10000000
 */

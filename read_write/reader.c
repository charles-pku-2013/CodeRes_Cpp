// 演示 read 实际读入字节与期望不一样


#include <unistd.h>
#include <stdio.h>

#define BUFSIZE     1024

int main()
{
    char buf[BUFSIZE];

    ssize_t n;
    while( (n = read(STDIN_FILENO, buf, BUFSIZE)) > 0 ) {
        printf("read %zd bytes: ", n);
        for( ssize_t i = 0; i < n; ++i )
            printf("%02u", (unsigned char)buf[i]);
        putchar('\n');
        sleep(10); //!! 只要缓冲区不满，writer 不会因为 reader 的睡眠而阻塞，会一直向缓冲区写，直到满
    } // while

    return 0;
}

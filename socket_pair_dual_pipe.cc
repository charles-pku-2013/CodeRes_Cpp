#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define MSG_SIZE 100

int main() {
    // sv[0] 和 sv[1] 是创建的两个文件描述符（套接字描述符）
    int sv[2]; 
    pid_t pid;
    char buffer[MSG_SIZE];

    char p_message[] = "Hello Child, from Parent using socketpair!";
    char c_message[] = "Hello Parent, from Child using socketpair!";

    // 1. 创建一个全双工的 UNIX 域套接字对
    // AF_UNIX: 使用本地通信
    // SOCK_STREAM: 提供可靠的、面向连接的字节流（也可以使用 SOCK_DGRAM）
    // 0: 协议（通常为 0）
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair error");
        exit(EXIT_FAILURE);
    }

    // 2. Fork 进程
    pid = fork();

    if (pid < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // --- 子进程 ---
        // 3. 关闭子进程不需要的套接字端 (通常只保留一个描述符进行通信)
        close(sv[0]); 

        printf("Child: Sending message to parent...\n");
        write(sv[1], c_message, strlen(c_message) + 1);

        // 从父进程接收消息
        read(sv[1], buffer, sizeof(buffer));
        printf("Child: Received message: %s\n", buffer);

        // 关闭子进程使用的套接字
        close(sv[1]);
        exit(EXIT_SUCCESS);

    } else {
        // --- 父进程 ---
        // 3. 关闭父进程不需要的套接字端
        close(sv[1]); 

        // 从子进程接收消息
        read(sv[0], buffer, sizeof(buffer));
        printf("Parent: Received message: %s\n", buffer);

        printf("Parent: Sending message to child...\n");
        write(sv[0], p_message, strlen(p_message) + 1);

        // 关闭父进程使用的套接字
        close(sv[0]);

        // 等待子进程结束
        wait(NULL);
    }

    return 0;
}

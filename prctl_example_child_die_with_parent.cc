
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

// 信号处理函数
void signal_handler(int signum) {
    if (signum == SIGHUP) {
        printf("[子进程 %d]: 检测到父进程已终止 (SIGHUP)。正在退出。\n", getpid());
        exit(EXIT_SUCCESS);
    }
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork 失败");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // --- 子进程代码 ---

        // 设置信号处理函数以捕获 SIGHUP 信号
        signal(SIGHUP, signal_handler);

        // 核心调用：设置当父进程死亡时，向本子进程发送 SIGHUP 信号
        if (prctl(PR_SET_PDEATHSIG, SIGHUP) == -1) {
            perror("prctl(PR_SET_PDEATHSIG) 失败");
            exit(EXIT_FAILURE);
        }

        printf("[子进程 %d]: 已启动，并设置了 PDEATHSIG。正在等待父进程结束...\n", getpid());

        // 持续运行一段时间，模拟子进程的工作
        while (1) {
            sleep(1);
        }

        exit(EXIT_SUCCESS);

    } else {
        // --- 父进程代码 ---

        printf("[父进程 %d]: 已启动子进程 %d。\n", getpid(), pid);
        printf("[父进程 %d]: 运行 3 秒后即将退出。\n", getpid());

        // 父进程休眠 3 秒钟
        sleep(3);

        printf("[父进程 %d]: 正在退出。\n", getpid());
        // 父进程自然退出，内核将向子进程发送 SIGHUP 信号
    }

    return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdint>

// 辅助函数: 向管道写入数据
void write_to_pipe(int fd, const void* data, size_t size) {
    if (write(fd, data, size) != static_cast<ssize_t>(size)) {
        perror("write_to_pipe failed");
        exit(EXIT_FAILURE);
    }
}

// 辅助函数: 从管道读取数据
void read_from_pipe(int fd, void* data, size_t size) {
    if (read(fd, data, size) != static_cast<ssize_t>(size)) {
        perror("read_from_pipe failed");
        exit(EXIT_FAILURE);
    }
}

// 序列化 vector<string> 并写入管道
void serialize_and_send(int write_fd, const std::vector<std::string>& data) {
    // 1. 发送 vector 的大小
    uint32_t vec_size = static_cast<uint32_t>(data.size());
    write_to_pipe(write_fd, &vec_size, sizeof(vec_size));

    // 2. 遍历每个字符串，发送其长度和内容
    for (const std::string& s : data) {
        uint32_t str_len = static_cast<uint32_t>(s.length());
        write_to_pipe(write_fd, &str_len, sizeof(str_len));
        if (str_len > 0) {
            write_to_pipe(write_fd, s.c_str(), str_len);
        }
    }
}

// 从管道读取数据并反序列化恢复 vector<string>（零拷贝优化）
std::vector<std::string> receive_and_deserialize(int read_fd) {
    std::vector<std::string> data;

    // 1. 读取 vector 的大小
    uint32_t vec_size;
    read_from_pipe(read_fd, &vec_size, sizeof(vec_size));
    data.reserve(vec_size);

    // 2. 循环读取每个字符串的长度和内容
    for (uint32_t i = 0; i < vec_size; ++i) {
        uint32_t str_len;
        read_from_pipe(read_fd, &str_len, sizeof(str_len));

        if (str_len > 0) {
            std::string s;
            s.resize(str_len); // 关键步骤：调整string大小，分配内部缓冲区

            // 直接将数据从管道读入string的内部缓冲区
            // s.data() 返回的指针指向可写的内部存储空间 (C++17及以后)
            // 在 C++11/14 中可以使用 &s[0]
            read_from_pipe(read_fd, s.data(), str_len); 
            
            data.emplace_back(std::move(s)); // 将构建好的string移动到vector中
        } else {
            data.emplace_back("");
        }
    }
    return data;
}

int main() {
    int pipefd[2]; // pipefd[0] for read, pipefd[1] for write
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // 子进程
        close(pipefd[1]); // 关闭写入端
        std::vector<std::string> received_vec = receive_and_deserialize(pipefd[0]);
        close(pipefd[0]); // 关闭读取端

        std::cout << "子进程收到数据：" << std::endl;
        for (size_t i = 0; i < received_vec.size(); ++i) {
            std::cout << "  string[" << i << "]: " << received_vec[i] << std::endl;
        }

    } else { // 父进程
        close(pipefd[0]); // 关闭读取端
        std::vector<std::string> original_vec = {"Hello", "World", "Inter-Process", "Communication"};
        
        serialize_and_send(pipefd[1], original_vec);
        close(pipefd[1]); // 关闭写入端，通知子进程数据发送完毕

        // 等待子进程结束
        wait(NULL);
        std::cout << "父进程发送完毕并退出。" << std::endl;
    }

    return 0;
}

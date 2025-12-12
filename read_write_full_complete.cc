/**
 * write_fully: 确保所有期望的字节都被写入（处理短写）。
 *
 * @param fd: 文件描述符。
 * @param buf: 待写入数据的缓冲区。
 * @param count: 期望写入的字节数。
 * @return: 成功写入的字节总数（应等于 count），
 *          如果遇到无法恢复的错误，则返回 -1 并设置 errno。
 */
inline ssize_t write_fully(int fd, const void* buf, size_t count) {
    size_t      total_written = 0;
    const char* ptr = static_cast<const char*>(buf);

    while (total_written < count) {
        // 尝试写入剩余的数据
        ssize_t bytes_written = write(fd, ptr + total_written, count - total_written);

        if (bytes_written >= 0) {
            // 成功写入部分或全部数据，更新计数器
            total_written += bytes_written;
        } else {
            // 发生错误
            if (errno == EINTR) {
                // 如果是信号中断（EINTR），继续循环重试，不报错
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 如果是非阻塞 I/O 导致缓冲区满，这里可以选择阻塞等待或返回已写入的部分
                // 在这个“write_fully”的实现中，我们假设用户希望阻塞直到完成。
                // 但标准实现通常认为这是一个需要用户处理的场景。
                // 对于本示例的同步阻塞 I/O 目的，我们继续尝试：
                continue;
            } else {
                // 遇到无法恢复的错误（如 EBADF、EFBIG、ENOSPC 等）
                return -1;
            }
        }
    }

    return total_written;  // 返回写入的总字节数，此时应等于 count
}

/**
 * read_fully: 确保所有期望的字节都被读取（处理短读），除非到达 EOF。
 *
 * @param fd: 文件描述符。
 * @param buf: 存放读取数据的缓冲区。
 * @param count: 期望读取的字节数。
 * @return: 成功读取的字节总数。如果到达文件末尾（EOF）导致无法读满，则返回已读字节数（可能为 0 到 count-1）。
 *          如果遇到无法恢复的错误，则返回 -1 并设置 errno。
 */
inline ssize_t read_fully(int fd, void* buf, size_t count) {
    size_t total_read = 0;
    char*  ptr = static_cast<char*>(buf);

    while (total_read < count) {
        // 尝试读取剩余的数据
        ssize_t bytes_read = read(fd, ptr + total_read, count - total_read);

        if (bytes_read > 0) {
            // 成功读取部分数据，更新计数器
            total_read += bytes_read;
        } else if (bytes_read == 0) {
            // 到达文件末尾 (EOF)，提前终止循环
            break;
        } else {
            // 发生错误
            if (errno == EINTR) {
                // 信号中断，继续重试
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 非阻塞 I/O，目前无数据可读，继续重试（或者用户应处理阻塞逻辑）
                continue;
            } else {
                // 遇到无法恢复的错误
                return -1;
            }
        }
    }

    // 返回读取的总字节数。调用者应检查此值是否等于期望的 count。
    return total_read;
}

#define READ(fd, buf, sz)                                                                          \
    if (read_fully((fd), (buf), (sz)) != (ssize_t)(sz)) {                                          \
        throw std::runtime_error(fmt::format("read error at {}:{} size={} {}", __FILE__, __LINE__, \
                                             sz, strerror(errno)));                                \
    }

#define WRITE(fd, buf, sz)                                                                \
    if (write_fully((fd), (buf), (sz)) != (ssize_t)(sz)) {                                \
        throw std::runtime_error(fmt::format("write error at {}:{} size={} {}", __FILE__, \
                                             __LINE__, sz, strerror(errno)));             \
    }

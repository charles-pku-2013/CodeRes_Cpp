#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>  
#include <sys/shm.h>  
#include "shm_buffer.h"

// 共享内存在系统中的标识，不同的进程可以凭借这个标识找到这块内存并映射到自己的地址空间
#define IPC_KEY     0x1234

typedef ShmBuffer<BUFSIZ>   SharedBuffer;

#endif


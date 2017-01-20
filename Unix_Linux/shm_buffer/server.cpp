#include "common.h"

using namespace std;

static
void serve(SharedBuffer *pShmBuf)
{
    string respStr;     // 用于回复client

    while (true) {
        respStr.clear();
        pthread_mutex_lock(&(pShmBuf->mtxServer));
        // 等待client的数据
        while (!pShmBuf->clientReady)
            pthread_cond_wait(&(pShmBuf->condServer), &(pShmBuf->mtxServer));
        // 收到client数据，构造回复内容
        respStr.append("Server response: ").append(pShmBuf->buf);
        // 将回复内容写入共享buffer
        strncpy(pShmBuf->buf, respStr.c_str(), BUFSIZ);
        // 通知client数据已准备好
        pShmBuf->serverReady = true;
        pShmBuf->clientReady = false;
        pthread_mutex_unlock(&(pShmBuf->mtxServer));
        pthread_cond_signal(&(pShmBuf->condClient));
    } // while
}


int main()
{
    int id = -1;
    void *ptr = NULL;

    // 建立共享内存
	id = shmget((key_t)IPC_KEY, sizeof(SharedBuffer), 0666 | IPC_CREAT);
    assert(id >= 0);

    // 获取共享内存的起始地址
    ptr = shmat(id, NULL, 0);
    assert(ptr);

    // 在共享内存上建立ShmBuffer数据结构并初始化(构造函数)
    new (ptr) SharedBuffer;

    serve((SharedBuffer*)ptr);

    // 删除共享内存
    shmctl(id, IPC_RMID, 0);  

    return 0;
}

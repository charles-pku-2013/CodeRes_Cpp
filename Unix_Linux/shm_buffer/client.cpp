#include "common.h"

using namespace std;

static
void client_routine(SharedBuffer *pShmBuf)
{
    string reqStr;
    while (getline(cin, reqStr)) {
        // 从终端读入一行文字并复制到shared buffer中
        strncpy(pShmBuf->buf, reqStr.c_str(), BUFSIZ);
        // 通知server，client的数据已OK
        pShmBuf->clientReady = true;
        pthread_cond_signal(&(pShmBuf->condServer));

        // 等待server回复
        pthread_mutex_lock(&(pShmBuf->mtxClient));
        while (!pShmBuf->serverReady)
            pthread_cond_wait(&(pShmBuf->condClient), &(pShmBuf->mtxClient));
        cout << pShmBuf->buf << endl;
        pShmBuf->serverReady = false;
        pthread_mutex_unlock(&(pShmBuf->mtxClient));
    } // while
}

int main()
{
    int id = -1;
    void *ptr = NULL;

    // 找到由server建立的共享内存
	id = shmget((key_t)IPC_KEY, 0, 0);
    assert(id > 0);

    ptr = shmat(id, NULL, 0);
    assert(ptr);

    // 共享内存里存放的是已由server建好的ShmBuffer数据结构
    // bool mutex condvar的改变client server都看得见
    client_routine((SharedBuffer*)ptr);

    return 0;
}

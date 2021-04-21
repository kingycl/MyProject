//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELLBUFFER_H
#define TEST_CELLBUFFER_H

#include"Cell.h"

class CELLBuffer {
public:
    CELLBuffer(int nSize = 8192);

    ~CELLBuffer();

    char *data() {
        return _pBuff;
    }

    bool push(const char *pData, int nLen);

    void pop(int nLen);

    int write2socket(SOCKET sockfd);

    int read4socket(SOCKET sockfd);

    bool hasMsg();

    bool needWrite() {
        return _nLast > 0;
    }

private:
    //第二缓冲区 发送缓冲区
    char *_pBuff = nullptr;
    //可以用链表或队列来管理缓冲数据块
    //list<char*> _pBuffList;
    //缓冲区的数据尾部位置，已有数据长度
    size_t _nLast = 0;
    //缓冲区总的空间大小，字节长度
    size_t _nSize = 0;
    //缓冲区写满次数计数
    int _fullCount = 0;
};

#endif //TEST_CELLBUFFER_H

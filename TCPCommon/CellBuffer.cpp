//
// Created by king_ on 2021/4/18.
//

#include "CellBuffer.h"
#include "NetMessage.h"

CELLBuffer::CELLBuffer(int nSize) {
    _nSize = nSize;
    _pBuff = new char[_nSize];
}

CELLBuffer::~CELLBuffer() {
    if (_pBuff) {
        delete[] _pBuff;
        _pBuff = nullptr;
    }
}

bool CELLBuffer::push(const char *pData, int nLen) {
    if (_nLast + nLen <= _nSize) {
        //将要发送的数据 拷贝到发送缓冲区尾部
        memcpy(_pBuff + _nLast, pData, nLen);
        //计算数据尾部位置
        _nLast += nLen;

        if (_nLast == SEND_BUFF_SZIE) {
            ++_fullCount;
        }

        return true;
    } else {
        ++_fullCount;
    }

    return false;
}

void CELLBuffer::pop(int nLen) {
    int n = _nLast - nLen;
    if (n > 0) {
        memcpy(_pBuff, _pBuff + nLen, n);
    }
    _nLast = n;
    if (_fullCount > 0)
        --_fullCount;
}

int CELLBuffer::write2socket(SOCKET sockfd) {
    int ret = 0;
    //缓冲区有数据
    if (_nLast > 0 && INVALID_SOCKET != sockfd) {
        //发送数据
        ret = send(sockfd, _pBuff, _nLast, 0);
        //数据尾部位置清零
        _nLast = 0;
        //
        _fullCount = 0;
    }
    return ret;
}

int CELLBuffer::read4socket(SOCKET sockfd) {
    if (_nSize > _nLast) {
        //接收客户端数据
        char *szRecv = _pBuff + _nLast;
        int nLen = (int) recv(sockfd, szRecv, _nSize - _nLast, 0);
        //CELLLog::Info("nLen=%d\n", nLen);
        if (nLen <= 0) {
            return nLen;
        }
        //消息缓冲区的数据尾部位置后移
        _nLast += nLen;
        return nLen;
    }
    return 0;
}

bool CELLBuffer::hasMsg() {
    //判断消息缓冲区的数据长度大于消息头netmsg_DataHeader长度
    if (_nLast >= sizeof(netmsg_DataHeader)) {
        //这时就可以知道当前消息的长度
        auto *header = (netmsg_DataHeader *) _pBuff;
        //判断消息缓冲区的数据长度大于消息长度
        return _nLast >= header->dataLength;
    }
    return false;
}
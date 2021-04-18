//
// Created by king_ on 2021/4/18.
//

#include "CellClient.h"
#include "CellLog.h"

CELLClient::CELLClient(SOCKET sockfd, int sendSize, int recvSize) : _sendBuff(sendSize), _recvBuff(recvSize) {
    static int n = 1;
    id = n++;
    _sockfd = sockfd;

    resetDTHeart();
    resetDTSend();
}

CELLClient::~CELLClient() {
    CELLLog::Info("s=%d CELLClient%d.~CELLClient\n", serverId, id);
    if (INVALID_SOCKET != _sockfd) {
#ifdef _WIN32
        closesocket(_sockfd);
#else
        close(_sockfd);
#endif
        _sockfd = INVALID_SOCKET;
    }
}

int CELLClient::SendData(const char *pData, int len) {
    if (_sendBuff.push(pData, len)) {
        return len;
    }
    return SOCKET_ERROR;
}

//心跳检测
bool CELLClient::checkHeart(time_t dt) {
    _dtHeart += dt;
    if (_dtHeart >= CLIENT_HREAT_DEAD_TIME) {
        CELLLog::Info("checkHeart dead:s=%d,time=%ld\n", _sockfd, _dtHeart);
        return true;
    }
    return false;
}

//定时发送消息检测
bool CELLClient::checkSend(time_t dt) {
    _dtSend += dt;
    if (_dtSend >= CLIENT_SEND_BUFF_TIME) {
        //立即将发送缓冲区的数据发送出去
        SendDataReal();
        //重置发送计时
        resetDTSend();
        return true;
    }
    return false;
}
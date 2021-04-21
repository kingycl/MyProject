//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELLCLIENT_H
#define TEST_CELLCLIENT_H

#include "Cell.h"
#include "CellBuffer.h"
#include "NetMessage.h"

//客户端心跳检测死亡计时时间
#define CLIENT_HREAT_DEAD_TIME 60000
//在间隔指定时间后
//把发送缓冲区内缓存的消息数据发送给客户端
#define CLIENT_SEND_BUFF_TIME 200

//客户端数据类型
class CELLClient {
public:
    int id = -1;
    //所属serverid
    int serverId = -1;
public:
    CELLClient(SOCKET sockfd = INVALID_SOCKET, int sendSize = SEND_BUFF_SZIE, int recvSize = RECV_BUFF_SZIE);

    ~CELLClient();


    SOCKET sockfd() {
        return _sockfd;
    }

    int RecvData() {
        return _recvBuff.read4socket(_sockfd);
    }

    bool hasMsg() {
        return _recvBuff.hasMsg();
    }

    netmsg_DataHeader *front_msg() {
        return (netmsg_DataHeader *) _recvBuff.data();
    }

    void pop_front_msg() {
        if (hasMsg())
            _recvBuff.pop(front_msg()->dataLength);
    }

    bool needWrite() {
        return _sendBuff.needWrite();
    }

    //立即将发送缓冲区的数据发送给客户端
    int SendDataReal() {
        resetDTSend();
        return _sendBuff.write2socket(_sockfd);
    }

    //缓冲区的控制根据业务需求的差异而调整
    //发送数据
    int SendData(netmsg_DataHeader *header) {
        return SendData((const char *) header, header->dataLength);
    }

    int SendData(const char *pData, int len);

    void resetDTHeart() {
        _dtHeart = 0;
    }

    void resetDTSend() {
        _dtSend = 0;
    }

    //心跳检测
    bool checkHeart(time_t dt);

    //定时发送消息检测
    bool checkSend(time_t dt);

private:
    // socket fd_set  file desc set
    SOCKET _sockfd;
    //发送缓冲区
    CELLBuffer _sendBuff;
    //第二缓冲区 接收消息缓冲区
    CELLBuffer _recvBuff;
    //心跳死亡计时
    time_t _dtHeart;
    //上次发送消息数据的时间
    time_t _dtSend;
    //发送缓冲区遇到写满情况计数
    int _sendBuffFullCount = 0;
};

#endif //TEST_CELLCLIENT_H

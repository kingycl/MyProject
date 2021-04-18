//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_EASYTCPCLIENT_H
#define TEST_EASYTCPCLIENT_H

#include "Cell.h"
#include "CellClient.h"
#include "NetMessage.h"

class EasyTcpClient {
public:
    EasyTcpClient() {
        _isConnect = false;
    }

    virtual ~EasyTcpClient() {
        Close();
    }

    //初始化socket
    void InitSocket(int sendSize = SEND_BUFF_SZIE, int recvSize = RECV_BUFF_SZIE);

    //连接服务器
    int Connect(const char *ip, unsigned short port);

    //关闭套节字closesocket
    void Close();

    //处理网络消息
    bool OnRun();

    //是否工作中
    bool isRun() {
        return _pClient && _isConnect;
    }

    //接收数据 处理粘包 拆分包
    int RecvData(SOCKET cSock);

    //响应网络消息
    virtual void OnNetMsg(netmsg_DataHeader *header) = 0;

    //发送数据
    int SendData(netmsg_DataHeader *header) {
        if (isRun())
            return _pClient->SendData(header);
        return 0;
    }

    int SendData(const char *pData, int len) {
        if (isRun())
            return _pClient->SendData(pData, len);
        return 0;
    }

protected:
    CELLClient *_pClient = nullptr;
    bool _isConnect = false;
};

#endif //TEST_EASYTCPCLIENT_H

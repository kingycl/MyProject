//
// Created by king_ on 2021/6/14.
//

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#endif
#include "CommMsg.h"

class TcpClient {
public:
    TcpClient() = default;
    ~TcpClient() {
        Close();
    }

    bool InitSocket();
    bool Connect(const char *ip, unsigned short port);
    void Close();

    bool OnRun();
    bool IsRun();

    virtual void OnNetMsg(DataHeader *header);
    int ReceiveData();
    int SendData(DataHeader *header);

private:
    SOCKET m_sock = INVALID_SOCKET;

    static constexpr size_t MAX_RECV_BUFFER_LEN = 1024000;
    char m_dataBuffer[MAX_RECV_BUFFER_LEN] = {0};
    size_t m_lastPos = 0;
};

#endif// TCPCLIENT_H

//
// Created by king_ on 2021/6/14.
//

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "CommMsg.h"
#ifdef _WIN32
#define FD_SETSIZE 1024
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
#include <cstring>
#include <vector>

class TcpServer {
public:
    TcpServer() = default;
    virtual ~TcpServer() {
        Close();
    }

    bool InitSocket();
    bool Bind(const char *ip, unsigned short port) const;
    bool Listen(int n) const;
    bool Accept();
    void Close();
    bool OnRun();
    bool isRun() {
        return m_serverSock != INVALID_SOCKET;
    }

    virtual void OnNetMsg(SOCKET sock, DataHeader *header);
    int ReceiveData(SOCKET sock);
    int SendData(SOCKET sock, DataHeader *header);

private:
    std::vector<SOCKET> m_clients;
    SOCKET m_serverSock = INVALID_SOCKET;
};

#endif// TCPSERVER_H

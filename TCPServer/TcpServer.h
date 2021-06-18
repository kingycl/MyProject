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

constexpr size_t MAX_RECV_BUFFER_LEN = 102400;

class ClientSocket {
public:
    ClientSocket(SOCKET sock) : m_sock(sock) {}
    ~ClientSocket() = default;

    SOCKET GetSocket() {
        return m_sock;
    }
    char *GetBuffer() {
        return m_dataBuffer;
    }

    char *GetCurBuffer() {
        return m_dataBuffer + m_lastPos;
    }

    size_t GetPos() const {
        return m_lastPos;
    }

    void SetPos(size_t pos) {
        m_lastPos = pos;
    }

private:
    char m_dataBuffer[MAX_RECV_BUFFER_LEN] = {0};
    size_t m_lastPos = 0;
    SOCKET m_sock = INVALID_SOCKET;
};

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
    int ReceiveData(ClientSocket *client);
    int SendData(SOCKET sock, DataHeader *header);

private:
    std::vector<ClientSocket *> m_clients;
    SOCKET m_serverSock = INVALID_SOCKET;
};

#endif// TCPSERVER_H

//
// Created by king_ on 2021/6/14.
//

#include "TcpClient.h"
#include <iostream>
using namespace std;

bool TcpClient::InitSocket() {
#ifdef _WIN32
    WORD ver = MAKEWORD(2, 2);
    WSAData dat = {};
    WSAStartup(ver, &dat);
#endif

    if (m_sock != INVALID_SOCKET) {
        cout << "Close Old Connect" << endl;
        Close();
    }

    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) {
        cout << "Init Socket Error!" << endl;
        return false;
    }

    cout << "Init Socket Success!" << endl;
    return true;
}

bool TcpClient::Connect(const char *ip, unsigned short port) {
    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

#ifdef _WIN32
    sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
    sin.sin_addr.s_addr = inet_addr(ip);
#endif
    if (connect(m_sock, reinterpret_cast<sockaddr *>(&sin), sizeof(sin)) == SOCKET_ERROR) {
        cout << "Connect to server Error!" << endl;
        return false;
    }

    cout << "Connect to server Success!" << endl;
    return true;
}

void TcpClient::Close() {
#ifdef _WIN32
    closesocket(m_sock);
    WSACleanup();
#else
    close(m_sock);
#endif
    m_sock = INVALID_SOCKET;
}

bool TcpClient::OnRun() {
    while (IsRun()) {
        fd_set fdRead;

        FD_ZERO(&fdRead);
        FD_SET(m_sock, &fdRead);

        timeval t = {0, 0};
        int ret = select(m_sock + 1, &fdRead, nullptr, nullptr, &t);
        if (ret < 0) {
            printf("select exit!");
            return false;
        }

        if (FD_ISSET(m_sock, &fdRead)) {
            FD_CLR(m_sock, &fdRead);
            if (-1 == ReceiveData()) {
                printf("select任务结束2\n");
                return false;
            }
        }
    }
    return true;
}

bool TcpClient::IsRun() {
    return m_sock != INVALID_SOCKET;
}

void TcpClient::OnNetMsg(DataHeader *header) {
    switch (header->cmd) {
        case CMD_LOGIN_RESULT: {
            LoginResult *login = (LoginResult *) header;
            printf("收到服务端消息：CMD_LOGIN_RESULT,数据长度：%d \n", login->dataLength);
        } break;
        case CMD_LOGOUT_RESULT: {
            LogoutResult *logout = (LogoutResult *) header;
            printf("收到服务端消息：CMD_LOGOUT_RESULT,数据长度：%d \n", logout->dataLength);
        } break;
    }
}

int TcpClient::ReceiveData() {
    char szRecv[4096] = {};
    int len = recv(m_sock, szRecv, sizeof(DataHeader), 0);
    if (len < 0) {
        printf("与服务器断开连接，任务结束。\n");
        return -1;
    }

    DataHeader *header = (DataHeader *) szRecv;
    recv(m_sock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);

    OnNetMsg(header);
    return 0;
}

int TcpClient::SendData(DataHeader *header) {
    if (IsRun() && header) {
        return send(m_sock, reinterpret_cast<const char *>(header), header->dataLength, 0);
    }
    return SOCKET_ERROR;
}
//
// Created by king_ on 2021/6/14.
//

#include "TcpClient.h"
#include <cstring>
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

    cout << "建立Socket=<" << m_sock << ">成功..." << endl;
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
    cout << "<socket=" << m_sock << ">正在连接服务器<" << ip << ":" << port << ">..." << endl;
    if (connect(m_sock, reinterpret_cast<sockaddr *>(&sin), sizeof(sin)) == SOCKET_ERROR) {
        cout << "<socket=" << m_sock << ">错误，连接服务器<" << ip << ":" << port << ">失败..." << endl;
        return false;
    }

    cout << "<socket=" << m_sock << ">连接服务器<" << ip << ":" << port << ">成功..." << endl;
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
    if (IsRun()) {
        fd_set fdRead;

        FD_ZERO(&fdRead);
        FD_SET(m_sock, &fdRead);

        timeval t = {0, 0};
        int ret = select(m_sock + 1, &fdRead, nullptr, nullptr, &t);
        if (ret < 0) {
            cout << "select exit!" << endl;
            Close();
            return false;
        }

        if (FD_ISSET(m_sock, &fdRead)) {
            FD_CLR(m_sock, &fdRead);
            if (-1 == ReceiveData()) {
                cout << "select任务结束2" << endl;
                Close();
                return false;
            }
        }
        return true;
    }
    return false;
}

bool TcpClient::IsRun() {
    return m_sock != INVALID_SOCKET;
}

void TcpClient::OnNetMsg(DataHeader *header) {
    switch (header->cmd) {
        case CMD_LOGIN_RESULT: {
            LoginResult *login = (LoginResult *) header;
            cout << "收到服务端消息:CMD_LOGIN_RESULT,数据长度:" << login->dataLength << endl;
        } break;
        case CMD_LOGOUT_RESULT: {
            LogoutResult *logout = (LogoutResult *) header;
            cout << "收到服务端消息:CMD_LOGOUT_RESULT,数据长度:" << logout->dataLength << endl;
        } break;
    }
}

int TcpClient::ReceiveData() {
    int len = recv(m_sock, m_dataBuffer + m_lastPos, MAX_RECV_BUFFER_LEN - m_lastPos, 0);
    if (len < 0) {
        cout << "与服务器断开连接,任务结束." << endl;
        return -1;
    }
    m_lastPos += len;
    while (sizeof(DataHeader) <= m_lastPos) {
        DataHeader *header = (DataHeader *) m_dataBuffer;
        if (header->dataLength <= m_lastPos) {
            size_t size = m_lastPos - header->dataLength;
            OnNetMsg(header);
            memcpy(m_dataBuffer, m_dataBuffer + header->dataLength, size);
            m_lastPos -= header->dataLength;
        } else {
            break;
        }
    }

    return 0;
}

int TcpClient::SendData(DataHeader *header) {
    if (IsRun() && header) {
        return send(m_sock, reinterpret_cast<const char *>(header), header->dataLength, 0);
    }
    return SOCKET_ERROR;
}
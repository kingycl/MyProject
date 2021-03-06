//
// Created by king_ on 2021/6/14.
//

#include "TcpServer.h"
#include <iostream>

using namespace std;

bool TcpServer::InitSocket() {
#ifdef _WIN32
    WORD ver = MAKEWORD(2, 2);
    WSAData dat = {};
    WSAStartup(ver, &dat);
#endif

    if (m_serverSock != INVALID_SOCKET) {
        cout << "Close old socket!" << endl;
        Close();
    }

    m_serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSock == INVALID_SOCKET) {
        cout << "Init Socket Error!" << endl;
        return false;
    }

    cout << "Init Socket Success!" << endl;
    return true;
}

bool TcpServer::Bind(const char *ip, unsigned short port) const {
    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    uint32_t addr = ip ? inet_addr(ip) : INADDR_ANY;
#ifdef _WIN32
    sin.sin_addr.S_un.S_addr = addr;
#else
    sin.sin_addr.s_addr = addr;
#endif
    if (bind(m_serverSock, reinterpret_cast<sockaddr *>(&sin), sizeof(sin)) == SOCKET_ERROR) {
        cout << "Bind to port [4567] Error!" << endl;
        return false;
    }

    cout << "Bind to port [4567] Success!" << endl;
    return true;
}

bool TcpServer::Listen(int n) const {
    if (listen(m_serverSock, n) == SOCKET_ERROR) {
        cout << "Listen socket port error!" << endl;
        return false;
    }

    cout << "Listen socket port success!" << endl;
    return true;
}

bool TcpServer::Accept() {
    sockaddr_in clientAddr = {};
    int addrLen = sizeof(clientAddr);
    auto clientSock = INVALID_SOCKET;

#ifdef _WIN32
    clientSock = accept(m_serverSock, reinterpret_cast<sockaddr *>(&clientAddr), &addrLen);
#else
    clientSock = accept(m_serverSock, reinterpret_cast<sockaddr *>(&clientAddr), reinterpret_cast<socklen_t *>(&addrLen));
#endif
    if (clientSock == INVALID_SOCKET) {
        cout << "Accept client conn error!" << endl;
        return false;
    }
    cout << "New client connect : IP = " << inet_ntoa(clientAddr.sin_addr) << endl;

    m_clients.push_back(new ClientSocket(clientSock));
    return true;
}

void TcpServer::Close() {
    if (m_serverSock == INVALID_SOCKET) {
        return;
    }
#ifdef _WIN32
    for (auto sockTmp : m_clients) {
        closesocket(sockTmp->GetSocket());
        delete sockTmp;
    }
    closesocket(m_serverSock);
    WSACleanup();
#else
    for (auto sockTmp : m_clients) {
        close(sockTmp->GetSocket());
        delete sockTmp;
    }
    close(m_serverSock);
#endif
    m_clients.clear();
}

bool TcpServer::OnRun() {
    while (isRun()) {
        fd_set fdRead;
        fd_set fdWrite;
        fd_set fdExp;

        FD_ZERO(&fdRead);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdExp);

        FD_SET(m_serverSock, &fdRead);
        FD_SET(m_serverSock, &fdWrite);
        FD_SET(m_serverSock, &fdExp);

        SOCKET maxSocket = m_serverSock;
        for (auto client : m_clients) {
            FD_SET(client->GetSocket(), &fdRead);
            if (client->GetSocket() > maxSocket) {
                maxSocket = client->GetSocket();
            }
        }

        timeval t = {0, 0};
        int ret = select(maxSocket + 1, &fdRead, &fdWrite, &fdExp, &t);
        if (ret < 0) {
            cout << "select exit!" << endl;
            Close();
            return false;
        }
        if (FD_ISSET(m_serverSock, &fdRead)) {
            FD_CLR(m_serverSock, &fdRead);
            Accept();
        }
        for (int n = (int) m_clients.size() - 1; n >= 0; n--) {
            if (FD_ISSET(m_clients[n]->GetSocket(), &fdRead)) {
                if (-1 == ReceiveData(m_clients[n])) {
                    auto iter = m_clients.begin() + n;
                    if (iter != m_clients.end()) {
                        m_clients.erase(iter);
                    }
                }
            }
        }
    }
    return true;
}

int TcpServer::ReceiveData(ClientSocket *client) {
    int len = recv(client->GetSocket(), client->GetCurBuffer(), MAX_RECV_BUFFER_LEN - client->GetPos(), 0);
    if (len < 0) {
        cout << "与服务器断开连接,任务结束." << endl;
        return -1;
    }
    client->SetPos(client->GetPos() + len);
    while (sizeof(DataHeader) <= client->GetPos()) {
        DataHeader *header = (DataHeader *) client->GetBuffer();
        if (header->dataLength <= client->GetPos()) {
            size_t size = client->GetPos() - header->dataLength;
            OnNetMsg(client->GetSocket(), header);
            memcpy(client->GetBuffer(), client->GetBuffer() + header->dataLength, size);
            client->SetPos(client->GetPos() - header->dataLength);
        } else {
            break;
        }
    }

    return 0;
}

void TcpServer::OnNetMsg(SOCKET sock, DataHeader *header) {
    switch (header->cmd) {
        case CMD_LOGIN: {
            auto *login = (Login *) header;
            cout << "收到命令:CMD_LOGIN,数据长度:" << login->dataLength << ",userName=" << login->userName << " PassWord=" << login->PassWord << endl;
            //忽略判断用户密码是否正确的过程
            LoginResult ret;
            SendData(sock, (DataHeader *) &ret);
        } break;
        case CMD_LOGOUT: {
            auto *logout = (Logout *) header;
            cout << "收到命令:CMD_LOGOUT,数据长度:" << logout->dataLength << ",userName=" << logout->userName << endl;
            //忽略判断用户密码是否正确的过程
            LogoutResult ret;
            SendData(sock, (DataHeader *) &ret);
        } break;
        default: {
            DataHeader ret = {0, CMD_ERROR};
            SendData(sock, &ret);
        } break;
    }
}

int TcpServer::SendData(SOCKET sock, DataHeader *header) {
    if (isRun() && header) {
        return send(sock, reinterpret_cast<const char *>(header), header->dataLength, 0);
    }
    return SOCKET_ERROR;
}
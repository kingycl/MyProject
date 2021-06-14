//
// Created by king_ on 2021/6/13.
//

#include "MyServer.h"
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
#include <iostream>

using namespace std;

#include <vector>
std::vector<SOCKET> g_clients;

int processor(SOCKET sock) {
    //缓冲区
    char szRecv[4096] = {};
    // 5 接收客户端数据
    int nLen = recv(sock, szRecv, sizeof(DataHeader), 0);
    DataHeader *header = (DataHeader *) szRecv;
    if (nLen <= 0) {
        printf("客户端已退出，任务结束。\n");
        return -1;
    }
    switch (header->cmd) {
        case CMD_LOGIN: {
            recv(sock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            Login *login = (Login *) szRecv;
            printf("收到命令：CMD_LOGIN,数据长度：%d,userName=%s PassWord=%s\n", login->dataLength, login->userName, login->PassWord);
            //忽略判断用户密码是否正确的过程
            LoginResult ret;
            send(sock, (char *) &ret, sizeof(LoginResult), 0);
        } break;
        case CMD_LOGOUT: {
            recv(sock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            Logout *logout = (Logout *) szRecv;
            printf("收到命令：CMD_LOGOUT,数据长度：%d,userName=%s \n", logout->dataLength, logout->userName);
            //忽略判断用户密码是否正确的过程
            LogoutResult ret;
            send(sock, (char *) &ret, sizeof(ret), 0);
        } break;
        default: {
            DataHeader header = {0, CMD_ERROR};
            send(sock, (char *) &header, sizeof(header), 0);
        } break;
    }
    return 0;
}
int main() {
#ifdef _WIN32
    WORD ver = MAKEWORD(2, 2);
    WSAData dat = {};
    WSAStartup(ver, &dat);
#endif

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        cout << "Init Socket Error!" << endl;
    } else {
        cout << "Init Socket Success!" << endl;
    }

    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(4567);
#ifdef _WIN32
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
#else
    sin.sin_addr.s_addr = INADDR_ANY;
#endif
    if (bind(sock, reinterpret_cast<sockaddr *>(&sin), sizeof(sin)) == SOCKET_ERROR) {
        cout << "Bind to port [4567] Error!" << endl;
    } else {
        cout << "Bind to port [4567] Success!" << endl;
    }

    if (listen(sock, 5) == SOCKET_ERROR) {
        cout << "Listen socker port error!" << endl;
    } else {
        cout << "Listen socker port success!" << endl;
    }

    while (true) {
        fd_set fdRead;
        fd_set fdWrite;
        fd_set fdExp;

        FD_ZERO(&fdRead);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdExp);

        FD_SET(sock, &fdRead);
        FD_SET(sock, &fdWrite);
        FD_SET(sock, &fdExp);

        SOCKET maxSocket = sock;
        for (auto client : g_clients) {
            FD_SET(client, &fdRead);
            if (client > maxSocket) {
                maxSocket = client;
            }
        }

        timeval t = {0, 0};
        int ret = select(maxSocket + 1, &fdRead, &fdWrite, &fdExp, &t);
        if (ret < 0) {
            printf("select exit!");
            break;
        }
        if (FD_ISSET(sock, &fdRead)) {
            FD_CLR(sock, &fdRead);

            sockaddr_in clientAddr = {};
            int addrLen = sizeof(clientAddr);
            SOCKET clientSock = INVALID_SOCKET;

#ifdef _WIN32
            clientSock = accept(sock, reinterpret_cast<sockaddr *>(&clientAddr), &addrLen);
#else
            clientSock = accept(sock, reinterpret_cast<sockaddr *>(&clientAddr), reinterpret_cast<socklen_t *>(&addrLen));
#endif
            if (clientSock == INVALID_SOCKET) {
                cout << "Accept client conn error!" << endl;
            }
            cout << "New client connect : IP = " << inet_ntoa(clientAddr.sin_addr) << endl;

            g_clients.push_back(clientSock);
        }
        for (int n = (int) g_clients.size() - 1; n >= 0; n--) {
            if (FD_ISSET(g_clients[n], &fdRead)) {
                if (-1 == processor(g_clients[n])) {
                    auto iter = g_clients.begin() + n;
                    if (iter != g_clients.end()) {
                        g_clients.erase(iter);
                    }
                }
            }
        }
    }


#ifdef _WIN32
    for (auto sockTmp : g_clients) {
        closesocket(sockTmp);
    }
    closesocket(sock);
    WSACleanup();
#else
    for (auto sockTmp : g_clients) {
        close(sockTmp);
    }
    close(sock);
#endif

    return 0;
}
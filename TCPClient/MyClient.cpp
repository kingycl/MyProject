//
// Created by king_ on 2021/6/13.
//

#include "MyClient.h"
#include "CommMsg.h"
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
#include <cstring>
#include <iostream>
#include <thread>

using namespace std;

int Processor(SOCKET sock) {
    char szRecv[4096] = {};
    // 5 接收客户端数据
    int nLen = recv(sock, szRecv, sizeof(DataHeader), 0);
    DataHeader *header = (DataHeader *) szRecv;
    if (nLen <= 0) {
        printf("与服务器断开连接，任务结束。\n");
        return -1;
    }
    switch (header->cmd) {
        case CMD_LOGIN_RESULT: {
            recv(sock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            LoginResult *login = (LoginResult *) szRecv;
            printf("收到服务端消息：CMD_LOGIN_RESULT,数据长度：%d\n", login->dataLength);
        } break;
        case CMD_LOGOUT_RESULT: {
            recv(sock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            LogoutResult *logout = (LogoutResult *) szRecv;
            printf("收到服务端消息：CMD_LOGOUT_RESULT,数据长度：%d\n", logout->dataLength);
        } break;
    }
    return 0;
}

bool g_bRun = true;
void cmdThread(SOCKET sock) {
    while (true) {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if (0 == strcmp(cmdBuf, "exit")) {
            g_bRun = false;
            printf("退出cmdThread线程\n");
            break;
        } else if (0 == strcmp(cmdBuf, "login")) {
            Login login;
            strcpy(login.userName, "lyd");
            strcpy(login.PassWord, "lydmm");
            send(sock, (const char *) &login, sizeof(Login), 0);
        } else if (0 == strcmp(cmdBuf, "logout")) {
            Logout logout;
            strcpy(logout.userName, "lyd");
            send(sock, (const char *) &logout, sizeof(Logout), 0);
        } else {
            printf("不支持的命令。\n");
        }
    }
}

int main() {
#ifdef _WIN32
    WORD ver = MAKEWORD(2, 2);
    WSAData dat = {};
    WSAStartup(ver, &dat);
#endif

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cout << "Init Socket Error!" << endl;
    } else {
        cout << "Init Socket Success!" << endl;
    }

    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(4567);
#ifdef _WIN32
    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif
    if (connect(sock, reinterpret_cast<sockaddr *>(&sin), sizeof(sin)) == SOCKET_ERROR) {
        cout << "Connect to server Error!" << endl;
    } else {
        cout << "Connect to server Success!" << endl;
    }

    std::thread t1(cmdThread, sock);
    t1.detach();

    while (g_bRun) {
        fd_set fdRead;

        FD_ZERO(&fdRead);
        FD_SET(sock, &fdRead);

        timeval t = {0, 0};
        int ret = select(sock + 1, &fdRead, nullptr, nullptr, &t);
        if (ret < 0) {
            printf("select exit!");
            break;
        }

        if (FD_ISSET(sock, &fdRead)) {
            FD_CLR(sock, &fdRead);
            if (-1 == Processor(sock)) {
                printf("select任务结束2\n");
                break;
            }
        }
    }


#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    return 0;
}
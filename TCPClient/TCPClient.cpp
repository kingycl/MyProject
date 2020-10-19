//
// Created by king_ on 2020/10/18.
//

#include "TCPClient.h"

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<Windows.h>
#include<WinSock2.h>

#include <iostream>

using namespace std;

int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr_ = {};
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(4567);
    addr_.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    connect(sock, reinterpret_cast<sockaddr*>(&addr_), sizeof(sockaddr));

    while (true) {
        char cmdBuf[128] = {};
        scanf_s("%s", cmdBuf, 128);
        send(sock, cmdBuf, strlen(cmdBuf) + 1, 0);

        char recvBuf[128];
        int rlen = recv(sock, recvBuf, sizeof(recvBuf), 0);
        if (rlen <= 0) {
            printf("Error MSG.\n");
            break;
        }
        printf("receive msg len : %d , msg : %s.\n", rlen, recvBuf);
    }
    char recvBuf[128];
    int len = recv(sock, recvBuf, sizeof(recvBuf), 0);
    printf("receive msg len : %d , msg : %s.\n", len, recvBuf);

    closesocket(sock);

    WSACleanup();
    cout << "Hello Client!\n" << endl;

    return 0;
}
//
// Created by king_ on 2020/10/18.
//

#include "TCPServer.h"
#include "NetMessage.h"

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
    if (SOCKET_ERROR == bind(sock, reinterpret_cast<sockaddr*>(&addr_), sizeof(sockaddr))) {
        printf("bind port error \n");
    }

    if (SOCKET_ERROR == listen(sock, 10)) {
        printf("listen port error \n");
    }

    sockaddr_in clientaddr = {};
    int iAddrLen = sizeof(sockaddr);
    SOCKET sockClient = accept(sock, reinterpret_cast<sockaddr*>(&clientaddr), &iAddrLen);
    if (sockClient == INVALID_SOCKET) {
        printf("Accept invalid socket link \n");
    }

    char recvBuf[123] = {};

    while (true) {

        int rlen = recv(sockClient, recvBuf, 128, 0);
        if (rlen <= 0) {
            printf("Clent is exit!\n");
            break;
        }

        string str = "Hello Socket!!";
        int slen = 0;
        if (0 == strcmp(recvBuf, "getName")) {
            str = "Xiao Qiang!";
            slen = send(sockClient, str.c_str(), static_cast<int>(str.size()) + 1, 0);
            printf("Send Message Len: %d str's len: %d str: %s \n", slen, str.length(), str.c_str());
        } else if (0 == strcmp(recvBuf, "age")) {
            str = "38!";
            slen = send(sockClient, str.c_str(), static_cast<int>(str.size()) + 1, 0);
            printf("Send Message Len: %d str's len: %d str: %s \n", slen, str.length(), str.c_str());
        } else if (0 == strcmp(recvBuf, "exit")){
            printf("exit!\n");
            break;
        } else {
            str = "Unknown CMD!!";
            slen = send(sockClient, str.c_str(), static_cast<int>(str.size()) + 1, 0);
            printf("Send Message Len: %d str's len: %d str: %s \n", slen, str.length(), str.c_str());
        }
    }

    closesocket(sock);

    WSACleanup();
    cout << "Hello Server!\n" << endl;
    return 0;
}
//
// Created by king_ on 2021/6/13.
//

#include "MyClient.h"
#include "TcpClient.h"

#include <cstring>
#include <iostream>
#include <thread>

using namespace std;

void cmdThread(TcpClient *client) {
    while (true) {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if (0 == strcmp(cmdBuf, "exit")) {
            client->Close();
            printf("退出cmdThread线程\n");
            break;
        } else if (0 == strcmp(cmdBuf, "login")) {
            Login login;
            strcpy(login.userName, "lyd");
            strcpy(login.PassWord, "lydmm");
            client->SendData((DataHeader *) &login);
        } else if (0 == strcmp(cmdBuf, "logout")) {
            Logout logout;
            strcpy(logout.userName, "lyd");
            client->SendData((DataHeader *) &logout);
        } else {
            printf("不支持的命令。\n");
        }
    }
}

int main() {

    TcpClient client;
    client.InitSocket();
    client.Connect("127.0.0.1", 4567);

    std::thread t1(cmdThread, &client);
    t1.detach();

    client.OnRun();
    client.Close();

    printf("已退出。\n");
    return 0;
}
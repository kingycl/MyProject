//
// Created by king_ on 2021/6/13.
//

#include "MyClient.h"
#include "TcpClient.h"

#include <cstring>
#include <iostream>
#include <thread>

using namespace std;

bool IS_RUN = true;
void cmdThread() {
    while (true) {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if (0 == strcmp(cmdBuf, "exit")) {
            IS_RUN = false;
            cout << "退出cmdThread线程" << endl;
            break;
        } else {
            cout << "不支持的命令." << endl;
        }
    }
}

int main() {

    constexpr int cCount = 10;
    TcpClient *client[cCount];

    for (auto &n : client) {
        n = new TcpClient();
    }
    for (auto &n : client) {
        n->InitSocket();
        n->Connect("127.0.0.1", 4567);
    }

    std::thread t1(cmdThread);
    t1.detach();

    Login login;
    strcpy(login.userName, "lyd");
    strcpy(login.PassWord, "lydmm");
    while (IS_RUN) {
        for (auto &n : client) {
            n->OnRun();
            n->SendData(&login);
        }
    }

    for (auto &n : client) {
        n->Close();
    }

    cout << "已退出." << endl;
    return 0;
}
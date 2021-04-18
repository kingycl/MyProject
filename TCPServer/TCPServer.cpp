//
// Created by king_ on 2020/10/18.
//

#include "MyServer.h"
#include "CellLog.h"

using namespace std;

int main() {
    CELLLog::Instance().setLogPath("serverLog.txt", "w");
    MyServer server;
    server.InitSocket();
    server.Bind(nullptr, 4567);
    server.Listen(64);
    server.Start(4);

    //在主线程中等待用户输入命令
    while (true) {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if (0 == strcmp(cmdBuf, "exit")) {
            server.Close();
            break;
        } else {
            CELLLog::Info("undefine cmd\n");
        }
    }

    CELLLog::Info("exit.\n");
    return 0;
}
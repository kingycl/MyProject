//
// Created by king_ on 2021/6/13.
//

#include "MyServer.h"
#include "TcpServer.h"

using namespace std;

int main() {
    TcpServer server;
    server.InitSocket();
    server.Bind(nullptr, 4567);
    server.Listen(5);
    server.OnRun();

    return 0;
}
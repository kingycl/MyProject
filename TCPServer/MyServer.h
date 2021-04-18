//
// Created by king_ on 2020/10/18.
//

#ifndef TEST_MYSERVER_H
#define TEST_MYSERVER_H

#include "EasyTcpServer.h"

class MyServer : public EasyTcpServer {
public:
    //cellServer 4 多个线程触发 不安全
    //如果只开启1个cellServer就是安全的
    virtual void OnNetJoin(CELLClient *pClient) {
        EasyTcpServer::OnNetJoin(pClient);
    }

    //cellServer 4 多个线程触发 不安全
    //如果只开启1个cellServer就是安全的
    virtual void OnNetLeave(CELLClient *pClient) {
        EasyTcpServer::OnNetLeave(pClient);
    }

    //cellServer 4 多个线程触发 不安全
    //如果只开启1个cellServer就是安全的
    virtual void OnNetMsg(CELLServer *pServer, CELLClient *pClient, netmsg_DataHeader *header);
};

#endif //TEST_MYSERVER_H

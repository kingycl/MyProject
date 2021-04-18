//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_MYCLIENT_H
#define TEST_MYCLIENT_H

#include "EasyTcpClient.h"
#include "NetMessage.h"

class MyClient : public EasyTcpClient {
public:
    //响应网络消息
    virtual void OnNetMsg(netmsg_DataHeader *header);

private:

};

#endif //TEST_MYCLIENT_H

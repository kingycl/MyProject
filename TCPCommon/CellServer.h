//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELLSERVER_H
#define TEST_CELLSERVER_H

#include "Cell.h"
#include "INetEvent.h"
#include "CellTask.h"
#include "CellThread.h"
#include "CellTime.h"

#include <map>
#include <vector>
#include <mutex>

//网络消息接收处理服务类
class EXPORT_DLL CELLServer {
public:
    CELLServer(int id);

    virtual ~CELLServer();

    void setEventObj(INetEvent *event);

    //关闭Socket
    void Close();

    //处理网络消息
    void OnRun(CELLThread *pThread);

    void CheckTime();

    void OnClientLeave(CELLClient *pClient);

    void WriteData(fd_set &fdWrite);

    void ReadData(fd_set &fdRead);

    //接收数据 处理粘包 拆分包
    int RecvData(CELLClient *pClient);

    //响应网络消息
    virtual void OnNetMsg(CELLClient *pClient, netmsg_DataHeader *header);

    void addClient(CELLClient *pClient);

    void Start();

    size_t getClientCount();

private:
    void ClearClients();

private:
    //正式客户队列
    std::map<SOCKET, CELLClient *> _clients;
    //缓冲客户队列
    std::vector<CELLClient *> _clientsBuff;
    //缓冲队列的锁
    std::mutex _mutex;
    //网络事件对象
    INetEvent *_pNetEvent;
    //
    CELLTaskServer _taskServer;
    //备份客户socket fd_set
    fd_set _fdRead_bak;
    //
    SOCKET _maxSock;
    //旧的时间戳
    time_t _oldTime = CELLTime::getNowInMilliSec();
    //
    CELLThread _thread;
    //
    int _id = -1;
    //客户列表是否有变化
    bool _clients_change = true;
};

#endif //TEST_CELLSERVER_H

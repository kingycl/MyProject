//
// Created by king_ on 2021/4/18.
//

#include "EasyTcpServer.h"
#include "CellLog.h"
#include "CellNetWork.h"

#include<mutex>

EasyTcpServer::EasyTcpServer() {
    _sock = INVALID_SOCKET;
    _recvCount = 0;
    _msgCount = 0;
    _clientCount = 0;
}

EasyTcpServer::~EasyTcpServer() {
    Close();
}
//初始化Socket
SOCKET EasyTcpServer::InitSocket() {
    CELLNetWork::Init();
    if (INVALID_SOCKET != _sock) {
        CELLLog::Info("warning, initSocket close old socket<%d>...\n", (int) _sock);
        Close();
    }
    _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == _sock) {
        CELLLog::Info("error, create socket failed...\n");
    } else {
        CELLLog::Info("create socket<%d> success...\n", (int) _sock);
    }
    return _sock;
}

//绑定IP和端口号
int EasyTcpServer::Bind(const char *ip, unsigned short port) {
    // 2 bind 绑定用于接受客户端连接的网络端口
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(port);//host to net unsigned short

#ifdef _WIN32
    if (ip){
        _sin.sin_addr.S_un.S_addr = inet_addr(ip);
    }
    else {
        _sin.sin_addr.S_un.S_addr = INADDR_ANY;
    }
#else
    if (ip) {
        _sin.sin_addr.s_addr = inet_addr(ip);
    } else {
        _sin.sin_addr.s_addr = INADDR_ANY;
    }
#endif
    int ret = bind(_sock, (sockaddr *) &_sin, sizeof(_sin));
    if (SOCKET_ERROR == ret) {
        CELLLog::Info("error, bind port<%d> failed...\n", port);
    } else {
        CELLLog::Info("bind port<%d> success...\n", port);
    }
    return ret;
}

//监听端口号
int EasyTcpServer::Listen(int n) {
    // 3 listen 监听网络端口
    int ret = listen(_sock, n);
    if (SOCKET_ERROR == ret) {
        CELLLog::Info("error, listen socket<%d> failed...\n", _sock);
    } else {
        CELLLog::Info("listen port<%d> success...\n", _sock);
    }
    return ret;
}

//接受客户端连接
SOCKET EasyTcpServer::Accept() {
    // 4 accept 等待接受客户端连接
    sockaddr_in clientAddr = {};
    int nAddrLen = sizeof(sockaddr_in);
    SOCKET cSock = INVALID_SOCKET;
#ifdef _WIN32
    cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
#else
    cSock = accept(_sock, (sockaddr *) &clientAddr, (socklen_t *) &nAddrLen);
#endif
    if (INVALID_SOCKET == cSock) {
        CELLLog::Info("error, accept INVALID_SOCKET...\n");
    } else {
        //将新客户端分配给客户数量最少的cellServer
        addClientToCELLServer(new CELLClient(cSock));
        //获取IP地址 inet_ntoa(clientAddr.sin_addr)
    }
    return cSock;
}

void EasyTcpServer::addClientToCELLServer(CELLClient *pClient) {
    //查找客户数量最少的CELLServer消息处理对象
    auto pMinServer = _cellServers[0];
    for (auto pServer : _cellServers) {
        if (pMinServer->getClientCount() > pServer->getClientCount()) {
            pMinServer = pServer;
        }
    }
    pMinServer->addClient(pClient);
}

void EasyTcpServer::Start(int nCELLServer) {
    for (int n = 0; n < nCELLServer; n++) {
        auto ser = new CELLServer(n + 1);
        _cellServers.push_back(ser);
        //注册网络事件接受对象
        ser->setEventObj(this);
        //启动消息处理线程
        ser->Start();
    }
    _thread.Start(nullptr,
                  [this](CELLThread *pThread) {
                      OnRun(pThread);
                  });
}

//关闭Socket
void EasyTcpServer::Close() {
    CELLLog::Info("EasyTcpServer.Close begin\n");
    _thread.Close();
    if (_sock != INVALID_SOCKET) {
        for (auto s : _cellServers) {
            delete s;
        }
        _cellServers.clear();
        //关闭套节字socket
#ifdef _WIN32
        closesocket(_sock);
#else
        close(_sock);
#endif
        _sock = INVALID_SOCKET;
    }
    CELLLog::Info("EasyTcpServer.Close end\n");
}

//处理网络消息
void EasyTcpServer::OnRun(CELLThread *pThread) {
    while (pThread->isRun()) {
        time4msg();
        //伯克利套接字 BSD socket
        fd_set fdRead;//描述符（socket） 集合
        //清理集合
        FD_ZERO(&fdRead);
        //将描述符（socket）加入集合
        FD_SET(_sock, &fdRead);
        ///nfds 是一个整数值 是指fd_set集合中所有描述符(socket)的范围，而不是数量
        ///既是所有文件描述符最大值+1 在Windows中这个参数可以写0
        timeval t = {0, 1};
        int ret = select(_sock + 1, &fdRead, 0, 0, &t); //
        if (ret < 0) {
            CELLLog::Info("EasyTcpServer.OnRun select exit.\n");
            pThread->Exit();
            break;
        }
        //判断描述符（socket）是否在集合中
        if (FD_ISSET(_sock, &fdRead)) {
            FD_CLR(_sock, &fdRead);
            Accept();
        }
    }
}

//计算并输出每秒收到的网络消息
void EasyTcpServer::time4msg() {
    auto t1 = _tTime.getElapsedSecond();
    if (t1 >= 1.0) {
        CELLLog::Info("thread<%d>,time<%lf>,socket<%d>,clients<%d>,recv<%d>,msg<%d>\n", (int) _cellServers.size(), t1,
                      _sock, (int) _clientCount, (int) (_recvCount / t1), (int) (_msgCount / t1));
        _recvCount = 0;
        _msgCount = 0;
        _tTime.update();
    }
}
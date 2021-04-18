//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELLNETWORK_H
#define TEST_CELLNETWORK_H

#include "Cell.h"

class EXPORT_DLL CELLNetWork {
private:
    CELLNetWork() {
#ifdef _WIN32
        //启动Windows socket 2.x环境
        WORD ver = MAKEWORD(2, 2);
        WSADATA dat;
        WSAStartup(ver, &dat);
#endif

#ifndef _WIN32
        //忽略异常信号，默认情况会导致进程终止
        signal(SIGPIPE, SIG_IGN);
#endif
    }

    ~CELLNetWork() {
#ifdef _WIN32
        //清除Windows socket环境
        WSACleanup();
#endif
    }

public:
    static void Init() {
        static CELLNetWork obj;
    }
};

#endif //TEST_CELLNETWORK_H

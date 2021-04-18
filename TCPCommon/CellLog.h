//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELLLOG_H
#define TEST_CELLLOG_H

#include "Cell.h"
#include "CellLog.h"
#include "CellTask.h"

class EXPORT_DLL CELLLog {
    //Info
    //Debug
    //Warring
    //Error
private:
    CELLLog();

    ~CELLLog();

public:
    static CELLLog &Instance();

    void setLogPath(const char *logPath, const char *mode);

    static void Info(const char *pformat, ...);

private:
    FILE *_logFile = nullptr;
    CELLTaskServer _taskServer;
};

#endif //TEST_CELLLOG_H

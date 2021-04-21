//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELLLOG_H
#define TEST_CELLLOG_H

#include "Cell.h"
#include "CellLog.h"
#include "CellTask.h"

class CELLLog {
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

    static void Info(const char *pStr) {
        CELLLog *pLog = &Instance();
        pLog->_taskServer.addTask([=]() {
            if (pLog->_logFile) {
                auto t = std::chrono::system_clock::now();
                auto tNow = std::chrono::system_clock::to_time_t(t);
                std::tm *now = std::gmtime(&tNow);
                fprintf(pLog->_logFile, "%s", "Info ");
                fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
                        now->tm_hour, now->tm_min, now->tm_sec);
                fprintf(pLog->_logFile, "%s", pStr);
                fflush(pLog->_logFile);
            }
            printf("%s", pStr);
        });
    }

    template<typename ...Args>
    static void Info(const char *pformat, Args ... args) {
        CELLLog *pLog = &Instance();
        pLog->_taskServer.addTask([=]() {
            if (pLog->_logFile) {
                auto t = std::chrono::system_clock::now();
                auto tNow = std::chrono::system_clock::to_time_t(t);

                std::tm *now = std::gmtime(&tNow);
                fprintf(pLog->_logFile, "%s", "Info ");
                fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
                        now->tm_hour, now->tm_min, now->tm_sec);
                fprintf(pLog->_logFile, pformat, args...);
                fflush(pLog->_logFile);
            }
            printf(pformat, args...);
        });
    }

private:
    FILE *_logFile = nullptr;
    CELLTaskServer _taskServer;
};

#endif //TEST_CELLLOG_H

//
// Created by king_ on 2021/4/18.
//

#include "CellLog.h"

#include <ctime>
#include <chrono>
#include <cstdarg>

CELLLog::CELLLog() {
    _taskServer.Start();
}

CELLLog::~CELLLog() {
    _taskServer.Close();
    if (_logFile) {
        Info("CELLLog fclose(_logFile)\n");
        fclose(_logFile);
        _logFile = nullptr;
    }
}

CELLLog &CELLLog::Instance() {
    static CELLLog sLog;
    return sLog;
}

void CELLLog::setLogPath(const char *logPath, const char *mode) {
    if (_logFile) {
        Info("CELLLog::setLogPath _logFile != nullptr\n");
        fclose(_logFile);
        _logFile = nullptr;
    }


    _logFile = fopen(logPath, mode);
    if (_logFile) {
        Info("CELLLog::setLogPath success,<%s,%s>\n", logPath, mode);
    } else {
        Info("CELLLog::setLogPath failed,<%s,%s>\n", logPath, mode);
    }
}

void CELLLog::Info(const char *pformat, ...) {
    CELLLog *pLog = &Instance();

    va_list pArg;
    va_start(pArg, pformat);
    pLog->_taskServer.addTask([=]() {
        if (pLog->_logFile) {
            auto t = std::chrono::system_clock::now();
            auto tNow = std::chrono::system_clock::to_time_t(t);

            std::tm *now = std::gmtime(&tNow);
            fprintf(pLog->_logFile, "%s", "Info ");
            fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
                    now->tm_hour, now->tm_min, now->tm_sec);
            fprintf(pLog->_logFile, pformat, pArg);
            fflush(pLog->_logFile);
        }
        printf(pformat, pArg);
    });
    va_end(pArg);
}
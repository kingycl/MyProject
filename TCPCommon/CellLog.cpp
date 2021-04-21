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
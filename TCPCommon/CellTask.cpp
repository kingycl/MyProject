//
// Created by king_ on 2021/4/18.
//

#include "CellTask.h"

void CELLTaskServer::OnRun(CELLThread *pThread) {
    while (pThread->isRun()) {
        //从缓冲区取出数据
        if (!_tasksBuf.empty()) {
            std::lock_guard<std::mutex> lock(_mutex);
            for (auto pTask : _tasksBuf) {
                _tasks.push_back(pTask);
            }
            _tasksBuf.clear();
        }
        //如果没有任务
        if (_tasks.empty()) {
            std::chrono::milliseconds t(1);
            std::this_thread::sleep_for(t);
            continue;
        }
        //处理任务
        for (auto pTask : _tasks) {
            pTask();
        }
        //清空任务
        _tasks.clear();
    }
    //处理缓冲队列中的任务
    for (auto pTask : _tasksBuf) {
        pTask();
    }
}
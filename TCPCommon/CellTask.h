//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELLTASK_H
#define TEST_CELLTASK_H

#include "Cell.h"
#include "CellThread.h"

#include <list>

//执行任务的服务类型
class EXPORT_DLL CELLTaskServer {
public:
    //所属serverid
    int serverId = -1;
private:
    typedef std::function<void()> CELLTask;
private:
    //任务数据
    std::list<CELLTask> _tasks;
    //任务数据缓冲区
    std::list<CELLTask> _tasksBuf;
    //改变数据缓冲区时需要加锁
    std::mutex _mutex;
    //
    CELLThread _thread;
public:
    //添加任务
    void addTask(CELLTask task) {
        std::lock_guard<std::mutex> lock(_mutex);
        _tasksBuf.push_back(task);
    }

    //启动工作线程
    void Start() {
        _thread.Start(nullptr, [this](CELLThread *pThread) {
            OnRun(pThread);
        });
    }

    void Close() {
        _thread.Close();
    }

protected:
    //工作函数
    void OnRun(CELLThread *pThread);
};

#endif //TEST_CELLTASK_H

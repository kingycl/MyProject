//
// Created by king_ on 2021/4/18.
//

#include "CellThread.h"

void CELLThread::Start(EventCall onCreate, EventCall onRun, EventCall onDestory) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_isRun) {
        _isRun = true;

        if (onCreate)
            _onCreate = onCreate;
        if (onRun)
            _onRun = onRun;
        if (onDestory)
            _onDestory = onDestory;

        //线程
        std::thread t(std::mem_fn(&CELLThread::OnWork), this);
        t.detach();
    }
}

//关闭线程
void CELLThread::Close() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_isRun) {
        _isRun = false;
        _sem.wait();
    }
}

//在工作函数中退出
//不需要使用信号量来阻塞等待
//如果使用会阻塞
void CELLThread::Exit() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_isRun) {
        _isRun = false;
    }
}

//线程的运行时的工作函数
void CELLThread::OnWork() {
    if (_onCreate)
        _onCreate(this);
    if (_onRun)
        _onRun(this);
    if (_onDestory)
        _onDestory(this);

    _sem.wakeup();
}
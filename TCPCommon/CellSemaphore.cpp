//
// Created by king_ on 2021/4/18.
//

#include "CellSemaphore.h"

void CELLSemaphore::wait() {
    std::unique_lock<std::mutex> lock(_mutex);
    if (--_wait < 0) {
        //阻塞等待
        _cv.wait(lock, [this]() -> bool {
            return _wakeup > 0;
        });
        --_wakeup;
    }
}

//唤醒当前线程
void CELLSemaphore::wakeup() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (++_wait <= 0) {
        ++_wakeup;
        _cv.notify_one();
    }
}
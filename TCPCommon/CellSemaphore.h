//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELLSEMAPHORE_H
#define TEST_CELLSEMAPHORE_H

#include "Cell.h"
#include <mutex>
#include <condition_variable>

//信号量
class CELLSemaphore {
public:
    //阻塞当前线程
    void wait();

    //唤醒当前线程
    void wakeup();

private:
    //改变数据缓冲区时需要加锁
    std::mutex _mutex;
    //阻塞等待-条件变量
    std::condition_variable _cv;
    //等待计数
    int _wait = 0;
    //唤醒计数
    int _wakeup = 0;
};

#endif //TEST_CELLSEMAPHORE_H

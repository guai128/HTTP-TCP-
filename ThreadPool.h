//
// Created by 25382 on 2023/11/23.
//

#ifndef HTTP_THREADPOOK_H
#define HTTP_THREADPOOK_H

#include <thread>
#include <functional>
#include <deque>
#include <string>
#include <mutex>

struct EasyError;

template <typename Function>
class ThreadPool {
    using Thread = std::thread;
    using TVector = std::vector<Thread>;
    using Mutex = std::mutex;
    using TDeque = std::deque<std::pair<Function, void*>>;
public:
    // 显示构造函数
    explicit ThreadPool(size_t = 5);
    ~ThreadPool();
    // 线程池是否满了
    bool isFull();
    // 添加新任务
    void addTask(Function&&, void*);
    // 更改线程池大小 只能大 不能小
    void resize(size_t);
    // 等待线程池所有任务完成
    // void wait();
    // 结束所有线程
//    void finish();
private:
    void _iniThreads();
    TVector _threads;
    TDeque _tasks;
    bool Stop; // 告诉线程退出
    Mutex _mutex; // 防止线程争夺任务
    size_t _curUsed;
    size_t _capacity;
};

struct EasyError : public std::exception {
    EasyError(std::string&& message)
            : errorMessage{message}
    {

    }

    const char * what() const noexcept override {
        return errorMessage.c_str();
    }
    std::string errorMessage;
};


#endif //HTTP_THREADPOOKL_H

//
// Created by 25382 on 2023/11/23.
//

#include "ThreadPool.h"

template <typename Function>
ThreadPool<Function>::ThreadPool(size_t size) :
        _capacity{size}, Stop{false}
{
    if (_capacity <= 0)
        throw EasyError("the size of threadPool must more than 1");
    _iniThreads();
}

template <typename Function>
void ThreadPool<Function>::_iniThreads() {
    for (int i=0; i<_capacity; ++i) {
        _threads.emplace_back([this]() {
            Function task;
            void * args;
            while (true) {
                if (Stop)
                    return;
                while (task == nullptr) {
                    // acquire
                    std::unique_lock<std::mutex> ulk(_mutex,std::defer_lock);
                    ulk.lock();
                    if (!_tasks.empty()) {
                        task = _tasks.front().first;
                        args = _tasks.front().second;
                        _tasks.pop_front();
                        ++_curUsed;
                    }
                    ulk.unlock();
                }
                task(args);
                _mutex.lock();
                --_curUsed;
                _mutex.unlock();
                task = nullptr;
            }
        });
    }
    _threads.shrink_to_fit();
}


template <typename Function>
void ThreadPool<Function>::addTask(Function&& func, void * args) {
    this->_tasks.push_back({func, args});
}

template <typename Function>
bool ThreadPool<Function>::isFull() {
    std::unique_lock<std::mutex> ulk(_mutex,std::defer_lock);
    ulk.lock();
    auto res = (_curUsed == _capacity);
    ulk.unlock();
    return res;
}

template <typename Function>
void ThreadPool<Function>::resize(size_t size) {
    std::unique_lock<std::mutex> ulk(_mutex,std::defer_lock);
    ulk.lock();
    this->_capacity = size;
    ulk.unlock();
}

template <typename Function>
ThreadPool<Function>::~ThreadPool(){
    Stop = true;

    for (auto & t : _threads)
        t.join();
};



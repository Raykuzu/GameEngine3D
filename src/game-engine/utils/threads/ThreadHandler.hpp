//
// Created by raykuzu on 20/01/2020.
//

#pragma once

#include <thread>
#include <future>
#include <condition_variable>

class ThreadHandler {
    public:
        ThreadHandler() = default;
        template<typename _Callable, typename _Object>
        explicit ThreadHandler(_Callable callable, _Object const &object) {
            _promiseStopThread = new std::promise<void>();
            _futureStopThread = _promiseStopThread->get_future();
            _thread = std::thread(callable, object, std::move(_futureStopThread));
        }
        ~ThreadHandler() = default;

        template<typename _Callable, typename _Object>
        void start(_Callable callable, _Object const &object) {
            _promiseStopThread = new std::promise<void>();
            _futureStopThread = _promiseStopThread->get_future();
            _thread = std::thread(callable, object, std::move(_futureStopThread));
        }
        void stop() {
            _promiseStopThread->set_value();
            _thread.join();
            delete _promiseStopThread;
        }
        void stop(std::condition_variable &cond) {
            _promiseStopThread->set_value();
            cond.notify_one();
            _thread.join();
            delete _promiseStopThread;
        }

    private:
        std::thread _thread;
        std::promise<void> *_promiseStopThread = nullptr;
        std::future<void> _futureStopThread;
};

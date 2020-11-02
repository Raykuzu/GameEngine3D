//
// Created by raykuzu on 20/01/2020.
//

#pragma once

#include <thread>
#include <future>
#include <condition_variable>
#include "ArcLogger.hpp"

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
        bool started() {
            return (_promiseStopThread != nullptr);
        }
        void stop() {
            if (_promiseStopThread == nullptr) {
                ArcLogger::warn("Please start to threadHandler before stopping it");
                return;
            }
            _promiseStopThread->set_value();
            _thread.join();
            delete _promiseStopThread;
            _promiseStopThread = nullptr;
        }
        void stop(std::condition_variable &cond) {
            if (_promiseStopThread == nullptr) {
                ArcLogger::warn("Please start to threadHandler before stopping it");
                return;
            }
            _promiseStopThread->set_value();
            cond.notify_one();
            _thread.join();
            delete _promiseStopThread;
            _promiseStopThread = nullptr;
        }

    private:
        std::thread _thread;
        std::promise<void> *_promiseStopThread = nullptr;
        std::future<void> _futureStopThread;
};

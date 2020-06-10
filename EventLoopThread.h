#pragma once
#include "EventLoop.h"
#include "Thread.h"
#include "noncopyable.h"
#include <mutex>
#include <condition_variable>
#include <thread>

class EventLoopThread : noncopyable
{
public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop *startLoop();

private:
  void threadFunc();
  EventLoop *loop_;
  bool exiting_;
  std::thread thread_;
  std::mutex mutex_;
  std::condition_variable cond_;
};
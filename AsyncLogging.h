#pragma once
#include <functional>
#include <string>
#include <vector>
#include "CountDownLatch.h"
#include "LogStream.h"
#include "Thread.h"
#include "noncopyable.h"
#include <mutex>
#include <condition_variable>
#include <chrono>

class AsyncLogging : noncopyable
{
public:
  AsyncLogging(const std::string basename, int flushInterval = 2);
  ~AsyncLogging()
  {
    if (running_)
      stop();
  }
  void append(const char *logline, int len);

  void start()
  {
    running_ = true;
    thread_.start();
    latch_.wait();
  }

  void stop()
  {
    running_ = false;
    cond_.notify_one();
    thread_.join();
  }

private:
  void threadFunc();
  typedef FixedBuffer<kLargeBuffer> Buffer;
  typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
  typedef std::shared_ptr<Buffer> BufferPtr;
  const int flushInterval_;
  bool running_;
  std::string basename_;
  Thread thread_;
  //MutexLock mutex_;
  std::mutex mutex_;
  //Condition cond_;
  std::condition_variable cond_;
  BufferPtr currentBuffer_;
  BufferPtr nextBuffer_;
  BufferVector buffers_;
  CountDownLatch latch_;
};
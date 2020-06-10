#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
    : mutex_(), cond_(), count_(count) {}

void CountDownLatch::wait()
{
  std::unique_lock<std::mutex> lock(mutex_);
  while (count_ > 0)
    cond_.wait(lock);
}

void CountDownLatch::countDown()
{
  std::lock_guard<std::mutex> lock(mutex_);
  --count_;
  if (count_ == 0)
    cond_.notify_all();
}
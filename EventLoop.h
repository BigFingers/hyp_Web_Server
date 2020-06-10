#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "Util.h"
#include "CurrentThread.h"
#include "Logging.h"
#include "Thread.h"
#include <mutex>

#include <iostream>
using namespace std;

class EventLoop
{
public:
  typedef std::function<void()> Functor;
  EventLoop();
  ~EventLoop();
  void loop();
  void quit();
  void runInLoop(Functor &&cb);
  void queueInLoop(Functor &&cb);
  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
  void assertInLoopThread() { assert(isInLoopThread()); }
  void shutdown(shared_ptr<Channel> channel) { shutDownWR(channel->getFd()); }
  void removeFromPoller(shared_ptr<Channel> channel)
  {
    poller_->epoll_del(channel);
  }
  void updatePoller(shared_ptr<Channel> channel, int timeout = 0)
  {
    poller_->epoll_mod(channel, timeout);
  }
  void addToPoller(shared_ptr<Channel> channel, int timeout = 0)
  {
    poller_->epoll_add(channel, timeout);
  }

private:
  // 声明顺序 wakeupFd_ > pwakeupChannel_
  bool looping_;
  shared_ptr<Epoll> poller_;
  int wakeupFd_;
  bool quit_;
  bool eventHandling_;
  mutable std::mutex mutex_;
  std::vector<Functor> pendingFunctors_;
  bool callingPendingFunctors_;
  const pid_t threadId_;
  shared_ptr<Channel> pwakeupChannel_;

  //wakeup函数作用是当有新连接到来时，由于当前线程可能正阻塞在epoll_wait中，因此需要被
  //唤醒，唤醒逻辑是往一个专用的fd中写入部分数据即可。
  void wakeup();

  void handleRead();
  void doPendingFunctors();
  void handleConn();
};

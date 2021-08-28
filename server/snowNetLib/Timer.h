#pragma once

#include "noncopyable.h"
#include "Timestamp.h"
#include "Callbacks.h"

#include <atomic>

class Timer : noncopyable
{
 public:
  Timer(TimerCallback cb, Timestamp when, double interval)
    : callback_(std::move(cb)),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(++numCreated_)
  { }

  void run() const
  {
    callback_();
  }

  Timestamp expiration() const  { return expiration_; }
  bool repeat() const { return repeat_; }
  int64_t sequence() const { return sequence_; }

  void restart(Timestamp now);

  static int numCreated() { return numCreated_; }

 private:
  const TimerCallback callback_;
  Timestamp expiration_;                    //过期时间
  const double interval_;                   //时间间隔
  const bool repeat_;         
  const int64_t sequence_;                  //在numCreated_队列中的下标

  static std::atomic_int numCreated_;       //已经创建了多少个timer
};
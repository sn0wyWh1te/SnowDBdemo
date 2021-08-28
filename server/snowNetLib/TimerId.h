#pragma once

class Timer;

class TimerId
{
public:
    friend class TimerQueue;

    TimerId()
    : timer_(nullptr),
        sequence_(0)
    {
    }

    TimerId(Timer* timer, int seq)
    : timer_(timer),
        sequence_(seq)
    {
    }

private:
    Timer* timer_;
    int sequence_;
};
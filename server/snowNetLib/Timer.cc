#include "Timer.h"

void Timer::restart(Timestamp now)
{
    if (repeat_)
    {
        expiration_ = Timestamp::addTime(now, interval_);
    }
    else
    {
        expiration_ = Timestamp();
    }
}

std::atomic_int Timer::numCreated_;
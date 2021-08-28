#pragma once

#include <iostream>
#include <string>

class Timestamp
{
public:
    Timestamp();
    explicit Timestamp(int64_t secondsSinceEpoch);
    static Timestamp now();
    std::string toString() const;
    int64_t getSeconds() const;
    static Timestamp addTime(Timestamp timestamp, int64_t seconds);

private:
    int64_t secondsSinceEpoch_;
};
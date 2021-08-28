#pragma once

#include <stdio.h>

#include "skipList.h"
#include "format.h"
#include "entry.h"

class SnowDB
{
public:
    SnowDB(const SnowDB &) = delete;
    bool operator=(const SnowDB &) = delete;

public:
    SnowDB();
    ~SnowDB();

    int init(const std::string &path = "./");
    int get(const std::string &key, std::string &value);
    int set(const std::string &key, const std::string &value);
    int remove(const std::string &key);

    void openDB(const std::string &path);
    void closeDB();
    int reclaim();

private:
    bool store(const std::string &key, const std::string &value, Entry::recordType type);

private:
    SkipList skipList_;

    // file operator
    int fileIndex_;
    int offset_;
    //file descriptor
    int fdWriter_;
    int fdCompact_;
};

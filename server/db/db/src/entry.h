#pragma once

#include "skipList.h"

#include <string>

class Entry
{
public:
    enum recordType
    {
        ADD = 0,
        MOD,
        DEL
    };

public:
    Entry() = default;
    explicit Entry(std::string key, std::string value, recordType type);
    std::string memToFile();
    int filetoMem(int fileIndex, int offset);
    void modSkipList(SkipList *skipList, int fileIndex, int offset);
    bool isExpired(SkipList *skipList, int index, int offset);

    int fileSize()
    {
        // entry format -->  key size + value size + key + value +recordtype
        return sizeof(int) + sizeof(int) + key_.size() + value_.size() + sizeof(recordType);
    }
    void setType(recordType type) { recordType_ = type; }
    std::string getKey() { return key_; }
    std::string getValue() { return value_; }

private:
    std::string key_;
    std::string value_;
    recordType recordType_;
};

#include <string.h>

#include "format.h"

SLKey::SLKey()
    : size_(0), key_(nullptr), fileIndex_(0), offset_(0)
{
}

SLKey::SLKey(std::string key, int fileIndex, int offset)
    : size_(key.size()), fileIndex_(fileIndex), offset_(offset)
{
    key_ = new char[size_];
    memcpy(key_, key.c_str(), size_);
}

SLKey::~SLKey()
{
    if (key_)
    {
        delete[] key_;
    }
}

SLKey::SLKey(SLKey &&tempKey)
{
    key_ = tempKey.getKeyPtr();
    size_ = tempKey.getKey().size();
    fileIndex_ = tempKey.getFileIndex();
    offset_ = tempKey.getOffset();
    tempKey.resetKey();
}

SLKey *SLKey::operator=(SLKey &&tempKey)
{
    key_ = tempKey.getKeyPtr();
    size_ = tempKey.getKey().size();
    fileIndex_ = tempKey.getFileIndex();
    offset_ = tempKey.getOffset();
    tempKey.resetKey();
    return this;
}

bool operator==(const SLKey &lhs, const SLKey &rhs)
{
    return lhs.getKey() == rhs.getKey();
}

bool operator<(const SLKey &lhs, const SLKey &rhs)
{
    return lhs.getKey() < rhs.getKey();
}

bool operator>(const SLKey &lhs, const SLKey &rhs)
{
    return lhs.getKey() > rhs.getKey();
}

bool operator!=(const SLKey &lhs, const SLKey &rhs)
{
    return lhs.getKey() != rhs.getKey();
}
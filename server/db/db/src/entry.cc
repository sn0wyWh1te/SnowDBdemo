#include "entry.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const static int HEADER = sizeof(Entry::recordType) + sizeof(int) + sizeof(int);

Entry::Entry(std::string key, std::string value, recordType type)
    : key_(key), value_(value), recordType_(type)
{
}

std::string Entry::memToFile()
{
    std::string entry;
    int keySize = key_.size();
    int valueSize = value_.size();

    // entry format -->  recordtype + key size + value size + key + value
    entry.append(reinterpret_cast<char *>(&recordType_), sizeof(Entry::recordType));
    entry.append(reinterpret_cast<char *>(&keySize), sizeof(int));
    entry.append(reinterpret_cast<char *>(&valueSize), sizeof(int));
    entry.append(key_.c_str(), key_.size());
    entry.append(value_.c_str(), value_.size());
    return entry;
}

int Entry::filetoMem(int fileIndex, int offset)
{
    std::string fileName("SkipListFile_");
    fileName += std::to_string(fileIndex);

    int fd = open(fileName.c_str(), O_RDONLY);
    if (fd == -1)
        return 1;
    int pos = lseek(fd, offset, SEEK_SET);
    if (pos != offset)
        return 1;

    char buf[HEADER]{};
    int readBytes = read(fd, buf, HEADER);
    if (readBytes != HEADER)
        return 1;

    recordType_ = *reinterpret_cast<recordType *>(buf);
    int keySize = *reinterpret_cast<recordType *>(buf + sizeof(recordType));
    int valueSize = *reinterpret_cast<recordType *>(buf + sizeof(recordType) + sizeof(int));

    char *context = new char[keySize + valueSize];
    readBytes = read(fd, context, sizeof(context));
    if (readBytes != sizeof(context))
    {
        close(fd);
        return 1;
    }

    key_.clear();
    key_.append(context, keySize);
    value_.clear();
    value_.append(context + keySize, valueSize);

    close(fd);
    return 0;
}

void Entry::modSkipList(SkipList *skipList, int fileIndex, int offset)
{
    SLKey temp(key_, fileIndex, offset);
    if (recordType_ == ADD || recordType_ == MOD)
    {
        skipList->insertElem(std::move(temp));
    }
    else if (recordType_ == DEL)
    {
        skipList->deleteElement(std::move(temp));
    }
}

bool Entry::isExpired(SkipList *skipList, int index, int offset)
{
    SLKey skipListKey(key_, 0, 0);
    int realIndex, realoffset;
    if (skipList->searchElement(std::move(skipListKey), &realIndex, &realoffset))
    {
        if (index == realIndex && realoffset == offset)
            return true;
    }
    return false;
}

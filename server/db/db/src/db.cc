#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>

#include "db.h"
#include "util.h"
#include "skipList.h"

const static int MAX_FILE_BTYES = 1073741824; //1G
const static int MAX_SKIPLIST_LEVEL = 6;

SnowDB::SnowDB()
    : fileIndex_(1), offset_(0), skipList_(MAX_SKIPLIST_LEVEL)
{
}

SnowDB::~SnowDB()
{
    closeDB();
}

int SnowDB::init(const std::string &path)
{
    std::string dbPath = path + "dbfile";
    int errNum = access(dbPath.c_str(), F_OK);
    if (errNum != 0)
    {
        if (mkdir(dbPath.c_str(), 0777))
            return 1;
    }

    if (chdir(dbPath.c_str()))
        return 1;

    openDB("./");
    std::string fileName("SkipListFile_");
    fileName += std::to_string(fileIndex_);
    fdWriter_ = open(fileName.c_str(), O_CREAT | O_RDWR);
    if (fdWriter_ == -1)
        return 1;
    return 0;
}

int SnowDB::remove(const std::string &key)
{
    SLKey skipListKey(key, 0, 0);
    if (skipList_.deleteElement(std::move(skipListKey)))
    {
        store(key, std::string(), Entry::DEL);
    }
    else
        return 1;
}

int SnowDB::get(const std::string &key, std::string &value)
{
    SLKey skipListKey(key, 0, 0);
    int fileIndex, offset;
    if (skipList_.searchElement(std::move(skipListKey), &fileIndex, &offset))
    {
        Entry temp;
        if (temp.filetoMem(fileIndex, offset))
            return 1;
        value = temp.getValue();
    }
    return 1;
}
/*
int SnowDB::set(const std::string &key, const std::string &value)
{
    // test whether the file size is enough to insert this record
    Entry::recordType type;
    Entry entry(key, value, type);
    int tempOffset = offset_ + entry.fileSize();
    //if file is not enough to insert this record, create a new file
    if (tempOffset > MAX_FILE_BTYES)
    {
        fileWriter_.close();
        fileIndex_++;
        std::string fileName("SkipListFile_");
        fileName += std::to_string(fileIndex_);
        fileWriter_.open(fileName);
        if (!fileWriter_.good())
            return 1;
        offset_ = 0;
        tempOffset = offset_ + entry.fileSize();
    }

    SLKey skipListKey(key, fileIndex_, offset_);
    int retCode = skipList_.insertElem(std::move(skipListKey));

    if (retCode == 1) //already exsits
        type = Entry::MOD;
    else
        type = Entry::ADD;
    entry.setType(type);

    fileWriter_ << entry.memToFile();
    fileWriter_.flush();
    offset_ = tempOffset;
    return 0;
}
*/

int SnowDB::set(const std::string &key, const std::string &value)
{
    SLKey skipListKey(key, fileIndex_, offset_);
    int retCode = skipList_.insertElem(std::move(skipListKey));
    if (retCode == 1) //already exsits
        store(key, value, Entry::MOD);
    else
        store(key, value, Entry::ADD);
    return 0;
}

void SnowDB::openDB(const std::string &path)
{
    std::vector<std::string> fileName;
    getDbFile(path, fileName);
    std::sort(fileName.begin(), fileName.end());

    int index = 0;
    int offset = 0;
    for (std::string &name : fileName)
    {
        size_t pos = name.find_first_of('_', 0);
        std::string fileIndex = name.substr(pos + 1);
        index = atoi(fileIndex.c_str());

        while (1)
        {
            Entry temp;
            if (temp.filetoMem(index, offset))
                break;
            temp.modSkipList(&skipList_, index, offset);
            offset += temp.fileSize();
        }
    }
    fileIndex_ = index + 1;
    offset_ = 0;
}

void SnowDB::closeDB()
{
    reclaim();
    close(fdWriter_);
}

int SnowDB::reclaim()
{
    std::vector<std::string> fileName;
    getDbFile("./", fileName);
    /* the last file is the current writing file,do not modify it */
    fileName.pop_back();

    /* compact index and offset */
    int pactIdx = 1;
    int pactOft = 0;

    if (fileName.empty())
        return 0;
    /* create compact file */
    std::string pactFileName = "PactFile_" + std::to_string(pactIdx);
    fdCompact_ = open(pactFileName.c_str(), O_CREAT | O_RDWR);
    if (fdCompact_ == -1)
        return 1;
    std::vector<std::string> vecCompactFile;
    vecCompactFile.push_back(pactFileName);

    for (std::string &name : fileName)
    {
        /* origin index and offset */
        int originOft = 0;
        int pos = name.find_first_of('_', 0);
        int originIdx = atoi(name.substr(pos + 1).c_str());

        Entry temp{};
        while (!temp.filetoMem(originIdx, originOft))
        {
            originOft += temp.fileSize();
            if (pactOft + temp.fileSize() > MAX_FILE_BTYES)
            {
                close(fdCompact_);
                pactIdx++;
                pactFileName = "PactFile_" + std::to_string(pactIdx);
                fdCompact_ = open(pactFileName.c_str(), O_CREAT | O_RDWR);
                if (fdCompact_ == -1)
                    return 1;
                pactOft = 0;
                vecCompactFile.push_back(pactFileName);
            }
            if (!temp.isExpired(&skipList_, originIdx, originOft))
            {
                size_t size = write(fdCompact_, temp.memToFile().c_str(), temp.fileSize());
                pactOft += temp.fileSize();
            }
        }
        if (unlink(name.c_str()))
            return 1;
    }
    close(fdCompact_);

    int nameIndex = fileIndex_ + 1;
    for (std::string &name : vecCompactFile)
    {
        std::string newName = std::string("SkipListFile_") + std::to_string(nameIndex);
        nameIndex++;
        if (rename(name.c_str(), newName.c_str()))
            return 1;
    }
    return 0;
}

bool SnowDB::store(const std::string &key, const std::string &value, Entry::recordType type)
{
    // test whether the file size is enough to insert this record
    Entry entry(key, value, type);
    int tempOffset = offset_ + entry.fileSize();
    //if file is not enough to insert this record, create a new file
    if (tempOffset > MAX_FILE_BTYES)
    {
        close(fdWriter_);
        fileIndex_++;
        std::string fileName("SkipListFile_");
        fileName += std::to_string(fileIndex_);
        fdWriter_ = open(fileName.c_str(), O_CREAT | O_RDWR);
        if (fdWriter_ == -1)
        {
            return 1;
        }
        offset_ = 0;
        tempOffset = offset_ + entry.fileSize();
    }

    size_t size = write(fdWriter_, entry.memToFile().c_str(), entry.fileSize());
    offset_ = tempOffset;
    return 0;
}

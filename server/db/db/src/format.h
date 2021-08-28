#pragma once

#include <string>

class SLKey
{
public:
    friend bool operator==(const SLKey &lhs, const SLKey &rhs);
    friend bool operator!=(const SLKey &lhs, const SLKey &rhs);
    friend bool operator<(const SLKey &lhs, const SLKey &rhs);
    friend bool operator>(const SLKey &lhs, const SLKey &rhs);

public:
    SLKey(const SLKey &) = delete;
    SLKey *operator=(const SLKey &) = delete;

public:
    SLKey();
    explicit SLKey(std::string key, int fileIndex, int offset);
    ~SLKey();
    SLKey(SLKey &&tempKey);
    SLKey *operator=(SLKey &&tempKey);
    std::string getKey() const { return std::string(key_, size_); }
    std::string getValue();
    char *getKeyPtr() { return key_; };
    void resetKey()
    {
        size_ = 0;
        key_ = nullptr;
        fileIndex_ = -1;
        offset_ = 0;
    }
    int getFileIndex() { return fileIndex_; }
    int getOffset() { return offset_; }
    void setFileIndex(int index) { fileIndex_ = index; }
    void setOffset(int offset) { offset_ = offset; }

private:
    int size_;
    char *key_;
    int fileIndex_;
    int offset_;
};

class Varint64
{
public:
    static char *EncodeVarint64(
        char *dst,     /* I/O: the begin of a buffer,it is assume that the buffer has enough space(at least 10 bytes) */
        uint64_t value /* I:the encode value */
    )
    /*
        description:
            output the end of the encode buffer.
    */
    {
        static const int B = 128;
        unsigned char *ptr = reinterpret_cast<unsigned char *>(dst);
        while (value >= B)
        {
            *(ptr++) = (value & (B - 1)) | B;
            value >>= 7;
        }
        *(ptr++) = static_cast<unsigned char>(value);
        return reinterpret_cast<char *>(ptr);
    }

    static const char *GetVarint64Ptr(
        const char *p,     /* I:the begin of the buffer */
        const char *limit, /* I:the end of the buffer  */
        uint64_t *value    /* O:the value */
    )
    /*
    description:
        output the end of the encode buffer.
    */
    {
        uint64_t result = 0;
        for (uint32_t shift = 0; shift <= 63 && p < limit; shift += 7)
        {
            uint64_t byte = *(reinterpret_cast<const unsigned char *>(p));
            p++;
            if (byte & 128)
            {
                // More bytes are present
                result |= ((byte & 127) << shift);
            }
            else
            {
                result |= (byte << shift);
                *value = result;
                return reinterpret_cast<const char *>(p);
            }
        }
        return NULL;
    }
};
#pragma once

#include <string>
#include <utility>

#include "format.h"

class Node
{
public:
    Node(const Node &) = delete;
    Node *operator=(const Node &lhs) = delete;

public:
    Node() = default;
    Node(SLKey &&key, int listLevel);
    Node(Node &&node)
    {
        forward_ = node.forward_;
        nodeLevel_ = node.nodeLevel_;
        key_ = std::move(key_);
    }

    Node *operator=(Node &&node)
    {
        forward_ = node.forward_;
        nodeLevel_ = node.nodeLevel_;
        key_ = std::move(key_);
        return this;
    }

    ~Node();
    SLKey *getKey() { return &(this->key_); }
    int getLevel() { return nodeLevel_; }

public:
    Node **forward_; // array stores the pointer to next node of each level
private:
    int nodeLevel_;
    SLKey key_;
};

class SkipList
{
public:
    class Iterator
    {
    public:
        Iterator(Node *node)
            : node_(node) {}

        Iterator next()
        {
            if (node_ != nullptr)
            {
                return Iterator(node_->forward_[0]);
            }
        }

        Iterator operator++(int)
        {
            if (node_ != nullptr)
            {
                node_ = node_->forward_[0];
            }
            return *this;
        }

        SLKey operator*()
        {
            SLKey *nodeKey = node_->getKey();
            std::string str = nodeKey->getKey();
            int index = nodeKey->getFileIndex();
            int offset = nodeKey->getOffset();

            return SLKey(str, index, offset);
        }

        bool operator==(const Iterator &lhs)
        {
            return node_ == lhs.node_;
        }

    private:
        Node *node_;
    };

public:
    SkipList(const SkipList &) = delete;
    bool operator=(const SkipList &) = delete;

public:
    SkipList(int maxLevel);
    ~SkipList();
    int getRandomLevel();
    Node *createNode(SLKey &&key, int level);
    int insertElem(SLKey &&key);
    void TravelAll();
    void TravelLowLevel();
    bool searchElement(SLKey &&key, int *fileIndex, int *offset);
    bool deleteElement(SLKey &&key);
    int size() { return elemCnt_; };
    Iterator begin() { return Iterator(header_); }
    Iterator end() { return Iterator(nullptr); }

private:
    int maxLevel_;      // Maximum level of the skip list
    int skipListLevel_; // current level of skip list
    Node *header_;      // pointer to header node
    int elemCnt_;       // skiplist current element count
};
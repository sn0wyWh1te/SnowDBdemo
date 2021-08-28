#include "skipList.h"

#include <iostream>
#include <string.h>

Node::Node(SLKey &&key, int Level)
    : key_(std::move(key))
{
    // level + 1, because array index is from 0 - level
    // and fill forward array with 0(NULL)
    forward_ = new Node *[Level + 1] {};
}

Node::~Node()
{
    if (forward_)
    {
        delete[] forward_;
    }
}

// create new node
Node *SkipList::createNode(SLKey &&key, int level)
{
    Node *n = new Node(std::move(key), level);
    return n;
}

// Insert given key in skip list
// return 1 means element exists
// return 0 means insert successfully
/* 
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+

*/
//return 0 if the key is new to insert
//return 1 if the key is exsists to modify
int SkipList::insertElem(SLKey &&key)
{
    Node *current = header_;

    // create update array and initialize it
    // update is array which put node that the node->forward_[i] should be operated later
    Node *update[maxLevel_ + 1];
    memset(update, 0, sizeof(Node *) * (maxLevel_ + 1));

    // start form highest level of skip list
    for (int i = skipListLevel_; i >= 0; i--)
    {
        while (current->forward_[i] != NULL && *(current->forward_[i]->getKey()) < key)
        {
            current = current->forward_[i];
        }
        update[i] = current;
    }

    // reached level 0 and forward pointer to right node, which is desired to insert key.
    current = current->forward_[0];

    // if current node have key equal to searched key, we modify it
    if (current != NULL && *(current->getKey()) == key)
    {
        std::cout << "key: " << key.getKey() << ", exists" << std::endl;
        SLKey *curKey = current->getKey();
        curKey->setFileIndex(key.getFileIndex());
        curKey->setOffset(key.getOffset());
        return 1;
    }

    // if current is NULL that means we have reached to end of the level
    // if current's key is not equal to key that means we have to insert node between update[0] and current node
    if (current == NULL || *(current->getKey()) != key)
    {
        // Generate a random level for node
        int randomLevel = getRandomLevel();

        // If random level is greater thar skip list's current level, initialize update value with pointer to header
        if (randomLevel > skipListLevel_)
        {
            for (int i = skipListLevel_ + 1; i < randomLevel + 1; i++)
            {
                update[i] = header_;
            }
            skipListLevel_ = randomLevel;
        }
        // create new node with random level generated
        Node *insertedNode = createNode(std::move(key), randomLevel);

        // insert node
        for (int i = 0; i <= randomLevel; i++)
        {
            insertedNode->forward_[i] = update[i]->forward_[i];
            update[i]->forward_[i] = insertedNode;
        }
        std::cout << "Successfully inserted key:" << key.getKey() << std::endl;
        elemCnt_++;
    }
    return 0;
}

// travel skip list all level for test
void SkipList::TravelAll()
{
    for (int i = 0; i <= skipListLevel_; i++)
    {
        Node *node = header_->forward_[i];
        std::cout << "Level " << i << ": ";
        while (node != NULL)
        {
            std::cout << node->getKey()->getKey() << ";";
            node = node->forward_[i];
        }
        std::cout << std::endl;
    }
}

// travel skip list the lowest level
void SkipList::TravelLowLevel()
{
    Node *node = header_->forward_[0];
    while (node != NULL)
    {
        std::cout << node->getKey()->getKey() << ";";
        node = node->forward_[0];
    }
}

// Delete element from skip list
bool SkipList::deleteElement(SLKey &&key)
{
    Node *current = header_;
    Node *update[maxLevel_ + 1];
    memset(update, 0, sizeof(Node *) * (maxLevel_ + 1));

    // start from highest level of skip list
    for (int i = skipListLevel_; i >= 0; i--)
    {
        while (current->forward_[i] != NULL && *(current->forward_[i]->getKey()) < key)
        {
            current = current->forward_[i];
        }
        update[i] = current;
    }

    current = current->forward_[0];
    if (current != NULL && *(current->getKey()) == key)
    {
        // start for lowest level and delete the current node of each level
        for (int i = 0; i <= skipListLevel_; i++)
        {
            // if at level i, next node is not target node, break the loop.
            if (update[i]->forward_[i] != current)
                break;
            update[i]->forward_[i] = current->forward_[i];
        }

        // Remove levels which have no elements
        while (skipListLevel_ > 0 && header_->forward_[skipListLevel_] == 0)
        {
            skipListLevel_--;
        }
        std::cout << "Successfully deleted key " << key.getKey() << std::endl;
        elemCnt_--;
        return true;
    }
    return false;
}

// Search for element in skip list
/*
                           +------------+
                           |  select 60 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |
level 3         1+-------->10+------------------>50+           70       100
                                                   |
                                                   |
level 2         1          10         30         50|           70       100
                                                   |
                                                   |
level 1         1    4     10         30         50|           70       100
                                                   |
                                                   |
level 0         1    4   9 10         30   40    50+-->60      70       100
*/
bool SkipList::searchElement(SLKey &&key, int *fileIndex, int *offset)
{
    std::cout << "search_element-----------------" << std::endl;
    Node *current = header_;

    // start from highest level of skip list
    for (int i = skipListLevel_; i >= 0; i--)
    {
        while (current->forward_[i] && *(current->forward_[i]->getKey()) < key)
        {
            current = current->forward_[i];
        }
    }

    //reached level 0 and advance pointer to right node, which we search
    current = current->forward_[0];

    // if current node have key equal to searched key, we get it
    if (current and *(current->getKey()) == key)
    {
        std::cout << "Found key: " << key.getKey() << std::endl;
        *fileIndex = current->getKey()->getFileIndex();
        *offset = current->getKey()->getOffset();
        return true;
    }

    std::cout << "Not Found Key:" << key.getKey() << std::endl;
    return false;
}

// construct skip list
SkipList::SkipList(int maxLevel)
    : maxLevel_(maxLevel), skipListLevel_(0), elemCnt_(0)
{
    // create header node and initialize key and value to null
    SLKey key;
    header_ = new Node(std::move(key), maxLevel);
}

SkipList::~SkipList()
{
    Node *node = header_->forward_[0];
    while (node != NULL)
    {
        Node *temp = node;
        node = node->forward_[0];
        delete temp;
    }
    if (header_)
    {
        delete header_;
    }
}

int SkipList::getRandomLevel()
{
    int k = 1;
    while (rand() % 2)
    {
        k++;
    }
    k = (k < maxLevel_) ? k : maxLevel_;
    return k;
};

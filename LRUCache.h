//
// Created by Christopher on 01/05/2017.
//

#ifndef LEETCODEHARD_LRUCACHE_H
#define LEETCODEHARD_LRUCACHE_H

#include <unordered_map>
#include <memory>
/** LRUCache data structure. The item least recently used will be removed if the cache
 *  size hits the cache capacity.
 */

/** Cache is key-value pair, this is the value class, which holds the value itself and
 * the position in the index circular array.
 */
struct Value {
    int value;
    int pos;
    Value(int v, int p) : value(v), pos(p) {}
};

/** A wrapper for holding the keys of the cache, because if use raw int array, we cannot
 * differentiate between if a slot in the key position array holds a key unless we use an
 * extra array to tell an empty slot.
 */
struct Pos {
    int key;
    Pos(int k) : key(k) {}
};

/** Use an unordered_map underneath to play the role of cache. Every time when a key is
 * either inserted, the key would be put at the tail of the key
 * position array, which is posArr; if a key is removed, the corresponding slot in posArr
 * would be set nullptr indicating that slot is empty; when a key is either updated or
 * retrieved, it would be put at the tail of the key position array and the original slot
 * would be set nullptr. If the tail is now at the end of the whole circular array, the
 * key position array is copied to a new array without any empty slots between the keys.
 * This LRUCache's put and get operations are both of amortized O(1) complexity.
 * Note that to achieve the amortized O(1) complexity, the arraySize should be greater than
 * the cache capacity. Usually, it is double of the cache capacity.
 */
class LRUCache {
private:
    /** posArr is an array of pointers to Pos objects */
    typedef Pos* PosPtr;
    int capacity;
    int arraySize;
    /** Use shared_ptr to avoid memory leak because the map doesn't own the pointers! */
    std::unordered_map<int, std::shared_ptr<Value>> cache;
    PosPtr * posArr;
    int head = 0, tail = 0;

    /** Reorganize the key position array without empty slots between keys */
    void adjust() {
        if ((tail + 1) % arraySize != head)
            return;
        PosPtr * newPosArr = new PosPtr[arraySize];
        int idx = 0;
        for (int i = head; i % arraySize != tail; i++) {
            if (posArr[i % arraySize] != nullptr) {
                newPosArr[idx] = posArr[i % arraySize];
                cache[newPosArr[idx]->key]->pos = idx;
                idx++;
            }
        }
        head = 0;
        tail = idx;
        delete[] posArr;
        posArr = newPosArr;
    }

    /** Move key postion array head to the next nonempty slot. */
    int findHead(int start) {
        int pos = (start + 1) % arraySize;
        while (posArr[pos] == nullptr) pos = (pos + 1) % arraySize;
        return pos;
    }

public:
    LRUCache(int capacity) {
        this->capacity = capacity;
        arraySize = capacity * 2;
        posArr = new PosPtr[arraySize];
    }

    /** Right now disable all copy and move constructors, re-add them in the future */
    explicit LRUCache(const LRUCache &) = delete;
    LRUCache & operator=(const LRUCache &) = delete;
    LRUCache(LRUCache &&) = delete;
    LRUCache & operator=(LRUCache &&) = delete;

    ~LRUCache() {
        delete[] posArr;
    }

    /** Get operation, put retrieved key at the end of the array. */
    int get(int key) {
        adjust();
        int res = -1;
        auto search = cache.find(key);
        if (search != cache.end()) {
            res = search->second->value;
            int pos = search->second->pos;
            posArr[pos] = nullptr;
            posArr[tail] = new Pos(key);
            cache[key]->pos = tail;
            tail = (tail + 1) % arraySize;
            if (head == pos) head = findHead(head);
        }
        return res;
    }

    /** Put operation, maintain the key position array per different conditions. */
    void put(int key, int value) {
        adjust();
        auto search = cache.find(key);
        if (search != cache.end()) {
            search->second->value = value;
            int pos = search->second->pos;
            posArr[pos] = nullptr;
            posArr[tail] = new Pos(key);
            cache[key]->pos = tail;
            tail = (tail + 1) % arraySize;
            if (head == pos) head = findHead(head);
        } else {
            if (cache.size() == capacity) {
                int lruKey = posArr[head]->key;
                cache.erase(lruKey);
                cache[key] = std::make_shared<Value>(value, tail);
                posArr[tail] = new Pos(key);
                tail = (tail + 1) % arraySize;
                head = findHead(head);
            } else {
                cache[key] = std::make_shared<Value>(value, tail);
                posArr[tail] = new Pos(key);
                tail = (tail + 1) % arraySize;
            }
        }
    }
};



#endif //LEETCODEHARD_LRUCACHE_H

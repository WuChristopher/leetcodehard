#include <iostream>
#include "LRUCache.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    LRUCache lruCache(5);
    lruCache.put(1, 1);
    for (int i = 2; i < 20; ++i) {
        lruCache.put(2, i);
    }
    for (int i = 0; i < 11; ++i) {
        std::cout << lruCache.get(i) << " ";
    }

    return 0;
}
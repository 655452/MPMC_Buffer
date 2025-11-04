#ifndef MPMC_RING_H
#define MPMC_RING_H

#include "ILockFreeRing.h"
#include "/home/asit/concurrentqueue/concurrentqueue.h"
#include <cstddef>

/**
 * @brief MPMC lock-free queue using Moodycamel ConcurrentQueue.
 * Supports multiple producers and multiple consumers.
 */
template<typename T>
class MpmcRing : public ILockFreeRing<T> {
public:
    explicit MpmcRing(size_t capacity = 1024)
        : queue(capacity) {}

    bool try_push(const T &item) override {
        return queue.try_enqueue(item);
    }

    bool try_pop(T &out) override {
        return queue.try_dequeue(out);
    }

private:
    moodycamel::ConcurrentQueue<T> queue;
};

#endif // MPMC_RING_H

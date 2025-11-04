#ifndef SPSC_RING_H
#define SPSC_RING_H

#include <atomic>
#include <vector>
#include "ILockFreeRing.h"
/**
 * @brief Lock-free Single Producer Single Consumer (SPSC) ring buffer.https://github.com/cameron314/concurrentqueue.git
 *
 * Designed for high-throughput real-time pipelines where
 * exactly one producer thread writes and one consumer thread reads.
 *Proces
 * Template parameter T = type of element stored.
 */
template<typename T>
class SpscRing  : public ILockFreeRing<T> {
public:
    explicit SpscRing(size_t capacity_pow2) {
        capacity = capacity_pow2;
        mask = capacity - 1;
        buffer.resize(capacity);
        head.store(0);
        tail.store(0);
    }

    bool try_push(const T &item) {
        size_t h = head.load(std::memory_order_relaxed);

        size_t next = (h + 1) & mask;
        if (next == tail.load(std::memory_order_acquire)) return false; // full
        buffer[h] = item;
        head.store(next, std::memory_order_release);
        return true;
    }

    bool try_pop(T &out) {
        size_t t = tail.load(std::memory_order_relaxed);
        if (t == head.load(std::memory_order_acquire)) return false; // empty
        out = buffer[t];
        tail.store((t + 1) & mask, std::memory_order_release);
        return true;
    }

private:
    size_t capacity, mask;
    std::vector<T> buffer;
    std::atomic<size_t> head, tail;
};

#endif // SPSC_RING_H

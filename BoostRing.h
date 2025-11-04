// BoostRing.h
#ifndef BOOST_RING_H
#define BOOST_RING_H

#include <boost/lockfree/spsc_queue.hpp>
#include "ILockFreeRing.h"

template<typename T>
class BoostRing : public ILockFreeRing<T> {
public:
    explicit BoostRing(size_t capacity) : queue(capacity) {}

    bool try_push(const T &item) override { return queue.push(item); }
    bool try_pop(T &out) override { return queue.pop(out); }

private:
    boost::lockfree::spsc_queue<T> queue;
};

#endif // BOOST_RING_H

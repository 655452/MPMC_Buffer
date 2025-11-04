// ILockFreeRing.h
#ifndef ILOCKFREERING_H
#define ILOCKFREERING_H

template<typename T>
class ILockFreeRing {
public:
    virtual ~ILockFreeRing() = default;

    virtual bool try_push(const T &item) = 0;
    virtual bool try_pop(T &out) = 0;
};

#endif // ILOCKFREERING_H

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QList>

template<class T>
class BlockingQueue
{
public:
    explicit BlockingQueue();

    void put(T item);
    T take();

private:
    QWaitCondition listIsEmpty;
    QMutex mutex;
    QList<T> list;
};

#endif // BLOCKINGQUEUE_H

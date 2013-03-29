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
    explicit BlockingQueue() {}

    void put(T item)
    {
        QMutexLocker locker(&mutex);
        list.append(item);
        listIsEmptyCondition.wakeOne();
        minSizeCondition.wakeAll();
    }

    T take()
    {
        QMutexLocker locker(&mutex);
        while(list.isEmpty())
            listIsEmptyCondition.wait(&mutex);
        T item = list.takeFirst();
        if (list.isEmpty())
            waitForEmptyCondition.wakeAll();
        return item;
    }

    QList<T> takeList()
    {
        QMutexLocker locker(&mutex);
        QList<T> copy(list);
        list.clear();
        waitForEmptyCondition.wakeAll();
        return copy;
    }

    void waitOnSize(int size)
    {
        QMutexLocker locker(&mutex);
        while(list.length() < size)
            minSizeCondition.wait(&mutex);
    }

    void waitForEmpty() {
        QMutexLocker locker(&mutex);
        while (!list.isEmpty())
            waitForEmptyCondition.wait(&mutex);
    }

    void clear() {
        QMutexLocker locker(&mutex);
        list.clear();
        waitForEmptyCondition.wakeAll();
    }

private:
    QWaitCondition minSizeCondition;
    QWaitCondition listIsEmptyCondition;
    QWaitCondition waitForEmptyCondition;
    QMutex mutex;
    QList<T> list;
};

#endif // BLOCKINGQUEUE_H

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
        return list.takeFirst();
    }

    QList<T> takeList()
    {
        QMutexLocker locker(&mutex);
        QList<T> copy(list);
        list.clear();
        return copy;
    }

    void waitOnSize(int size)
    {
        QMutexLocker locker(&mutex);
        while(list.length() < size)
            minSizeCondition.wait(&mutex);
    }

    void clear() {
        QMutexLocker locker(&mutex);
        list.clear();
    }

private:
    QWaitCondition minSizeCondition;
    QWaitCondition listIsEmptyCondition;
    QMutex mutex;
    QList<T> list;
};

#endif // BLOCKINGQUEUE_H

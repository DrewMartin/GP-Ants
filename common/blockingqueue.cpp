#include "blockingqueue.h"

template<class T>
BlockingQueue<T>::BlockingQueue()
{
}

template<class T>
void BlockingQueue<T>::put(T item)
{
    QMutexLocker locker(&mutex);
    list.append(item);
    listIsEmpty.wakeOne();
}

template<class T>
T BlockingQueue<T>::take()
{
    QMutexLocker locker(&mutex);
    while(list.isEmpty())
        listIsEmpty.wait(&mutex);
    return list.takeFirst();
}

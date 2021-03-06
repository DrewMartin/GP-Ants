#include "blockingcounter.h"

BlockingCounter::BlockingCounter(int count) :
    count(count),
    curr(0)
{
}

void BlockingCounter::wait()
{
    QMutexLocker locker(&mutex);
    while (curr < count)
        waiting.wait(&mutex);
    curr = 0;
}

void BlockingCounter::increment()
{
    QMutexLocker locker(&mutex);
    curr++;
    if (curr >= count)
        waiting.wakeAll();
}

void BlockingCounter::force()
{
    QMutexLocker locker(&mutex);
    curr = count;
    waiting.wakeAll();
}

void BlockingCounter::reset()
{
    QMutexLocker locker(&mutex);
    curr = 0;
}

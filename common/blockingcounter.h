#ifndef CYCLICBARRIER_H
#define CYCLICBARRIER_H

#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

class BlockingCounter
{
public:
    BlockingCounter(int count);

    void wait();
    void increment();
    void force();
    void reset();

private:
    const int count;
    int curr;
    QWaitCondition waiting;
    QMutex mutex;
};

#endif // CYCLICBARRIER_H

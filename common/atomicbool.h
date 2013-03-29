#ifndef ATOMICBOOL_H
#define ATOMICBOOL_H

#include <QMutex>
#include <QMutexLocker>

class AtomicBool
{
public:
    AtomicBool(bool initial);

    bool get();
    void set(bool to);

private:
    QMutex mutex;
    bool val;
};

#endif // ATOMICBOOL_H

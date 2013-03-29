#include "atomicbool.h"

AtomicBool::AtomicBool(bool initial) :
    val(initial)
{
}

bool AtomicBool::get()
{
    QMutexLocker locker(&mutex);
    return val;
}

void AtomicBool::set(bool to)
{
    QMutexLocker locker(&mutex);
    val = to;
}

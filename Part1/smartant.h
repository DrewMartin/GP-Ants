#ifndef SMARTANT_H
#define SMARTANT_H

#include "common/ant.h"

class SmartAnt : public Ant
{
public:
    SmartAnt(QPoint &location);
    virtual ~SmartAnt();

    bool update(QList<QList<Cell> > &cells);
};

#endif // SMARTANT_H

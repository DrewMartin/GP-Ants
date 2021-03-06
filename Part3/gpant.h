#ifndef GPANT_H
#define GPANT_H

#include "ant.h"

class GPAnt : public Ant
{
public:
    GPAnt(QPoint &location);
    virtual ~GPAnt();

    bool antHasFood();

    void setDirection(Direction dir);
    void moveForward();

    void turnRandom();
};

#endif // GPANT_H

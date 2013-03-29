#include "gpant.h"

GPAnt::GPAnt(QPoint &location) :
    Ant(location)
{
}

GPAnt::~GPAnt()
{
}

bool GPAnt::antHasFood()
{
    return hasFood;
}

void GPAnt::setDirection(Direction dir)
{
    direction = dir;
}


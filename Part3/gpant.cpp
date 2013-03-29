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

void GPAnt::moveForward()
{
    QPoint p = pointAfterForwardMove(direction);
    moveTo(p);
}

void GPAnt::turnRandom()
{
    int random;
    random = qrand() % 100;
    if (random >= 75)
        direction = directionAfterLeftTurn(direction);
    else if (random >= 50)
        direction = directionAfterRightTurn(direction);
}

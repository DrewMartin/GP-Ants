#include "pheremone.h"

Pheremone::Pheremone(QPoint &location, int initialStrength, int maxStrength, double decay) :
    Entity(location),
    strength(initialStrength),
    maxStrength(maxStrength),
    decayRate(decay)
{
}

Pheremone::~Pheremone()
{
}

QGraphicsItem *Pheremone::getGraphicsItem()
{
    return NULL;
}

void Pheremone::reset()
{
}

bool Pheremone::update()
{
    strength *= decayRate;
    return isAlive();
}

bool Pheremone::isAlive()
{
    return strength > 0;
}


int Pheremone::getStrength() const
{
    return strength;
}


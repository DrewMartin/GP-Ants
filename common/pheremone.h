#ifndef PHEREMONE_H
#define PHEREMONE_H

#include "entity.h"

class Pheremone : public Entity
{
public:
    Pheremone(QPoint &location, int initialStrength, int maxStrength, double decay);
    virtual ~Pheremone();

    virtual QGraphicsItem *getGraphicsItem();
    virtual void reset();

    bool update();
    bool isAlive();
    QPoint getDestination() const;
    int getStrength() const;

private:

    int strength;
    const int maxStrength;
    double decayRate;
};

#endif // PHEREMONE_H

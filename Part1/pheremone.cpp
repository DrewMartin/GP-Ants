#include "pheremone.h"

Pheremone::Pheremone(QPoint &location, int initialStrength, int maxStrength, double decay) :
    Entity(location),
    strength(initialStrength),
    maxStrength(maxStrength),
    decayRate(decay),
    color(0,255,0)
{
    rect = new QGraphicsRectItem(location.x(), location.y(), 1, 1);
    rect->setFlag(QGraphicsItem::ItemClipsToShape);
    rect->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    rect->setPen(QColor(0,0,0,0));
    rect->setZValue(-1);
    redraw();
}

Pheremone::~Pheremone()
{
    delete rect;
}

QGraphicsItem *Pheremone::getGraphicsItem()
{
    return rect;
}

void Pheremone::reset()
{
}

bool Pheremone::update()
{
    strength *= decayRate;
    redraw();
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


void Pheremone::redraw()
{
    int newVal = qMax(0, (maxStrength - strength)*255/maxStrength);
    color.setRed(newVal);
    color.setBlue(newVal);
//    color.setAlpha(newVal);
    rect->setBrush(QBrush(color));
}

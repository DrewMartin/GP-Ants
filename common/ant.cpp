#include "ant.h"
#include <QDebug>

const QColor Ant::color(255, 0, 0);
const QColor Ant::hasFoodColor(255, 180, 0);

Ant::Ant(QPoint &location) :
    Entity(location),
    initialLocation(location),
    ellipse(NULL),
    hasFood(false)
{
    switch(qrand() % 8) {
    case 0: direction = North; break;
    case 1: direction = NE; break;
    case 2: direction = East; break;
    case 3: direction = SE; break;
    case 4: direction = South; break;
    case 5: direction = SW; break;
    case 6: direction = West; break;
    case 7: direction = NW; break;
    }

    moveTo(location);
}

Ant::~Ant()
{
    if (ellipse)
        delete ellipse;
}

QGraphicsItem *Ant::getGraphicsItem()
{
    if (!ellipse) {
        ellipse = new QGraphicsEllipseItem();
        ellipse->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        ellipse->setFlag(QGraphicsItem::ItemClipsToShape);
        ellipse->setPen(QPen(color));
        ellipse->setBrush(QBrush(color));
        ellipse->setZValue(50);
    }
    return ellipse;
}

void Ant::reset()
{
    location = initialLocation;
}

Direction Ant::getDirection() const
{
    return direction;
}

void Ant::moveRandom()
{

    QPoint dest;
    Direction targetDir;
    int random;
    random = qrand() % 100;
    targetDir = direction;
    if (random >= 75)
        targetDir = directionAfterLeftTurn(direction);
    else if (random >= 50)
        targetDir = directionAfterRightTurn(direction);
    dest = pointAfterForwardMove(targetDir);
    direction = targetDir;
    moveTo(dest);
}

QPoint Ant::pointAfterForwardMove(Direction dir)
{
    QPoint dest(location);
    switch (dir) {
    case North: dest.setY(dest.y() + 1); break;
    case East: dest.setX(dest.x() + 1); break;
    case South: dest.setY(dest.y() - 1); break;
    case West: dest.setX(dest.x() - 1); break;
    case NE:
        dest.setY(dest.y() + 1);
        dest.setX(dest.x() + 1);
        break;
    case SE:
        dest.setY(dest.y() - 1);
        dest.setX(dest.x() + 1);
        break;
    case SW:
        dest.setY(dest.y() - 1);
        dest.setX(dest.x() - 1);
        break;
    case NW:
        dest.setY(dest.y() + 1);
        dest.setX(dest.x() - 1);
        break;
    }
    NORMALIZE_POINT(dest);
    return dest;
}

void Ant::takeFood()
{
    hasFood = true;
    direction = oppositeDirection(direction);
    if (ellipse){
        ellipse->setPen(QPen(hasFoodColor));
        ellipse->setBrush(QBrush(hasFoodColor));
    }
}

void Ant::dropFood()
{
    hasFood = false;
    if (ellipse){
        ellipse->setPen(QPen(color));
        ellipse->setBrush(QBrush(color));
    }
}

void Ant::moveTo(QPoint &p)
{
    if (p == location) {
        moveRandom();
        return;
    }
    location = p;
    if (ellipse)
        ellipse->setRect(LOC_HELPER(location,ANT_RAD));
}

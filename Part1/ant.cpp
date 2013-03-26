#include "ant.h"
#include <QDebug>

const QColor Ant::color(255, 0, 0);
const QColor Ant::hasFoodColor(255, 180, 0);

Ant::Ant(QPoint &location) :
    Entity(location),
    initialLocation(location),
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

    ellipse = new QGraphicsEllipseItem();
    ellipse->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    ellipse->setFlag(QGraphicsItem::ItemClipsToShape);
    ellipse->setPen(QPen(color));
    ellipse->setBrush(QBrush(color));
    ellipse->setZValue(50);
    moveTo(location);
}

Ant::~Ant()
{
    delete ellipse;
}

QGraphicsItem *Ant::getGraphicsItem()
{
    return ellipse;
}

void Ant::reset()
{
    location = initialLocation;
}

bool Ant::update(QList<QList<Cell> > &cells)
{
    if (hasFood) {
        if (cells[location.y()][location.x()].atAnthill()) {
            hasFood = false;
            ellipse->setPen(QPen(color));
            ellipse->setBrush(QBrush(color));
        } else {
            double scent, best;
            QPoint p = pointAfterForwardMove(direction);
            scent = best = cells[p.y()][p.x()].getScent();

            p = pointAfterForwardMove(directionAfterLeftTurn(direction));
            scent = cells[p.y()][p.x()].getScent();
            if (scent > best) {
                best = scent;
                direction = directionAfterLeftTurn(direction);
            }

            p = pointAfterForwardMove(directionAfterRightTurn(direction));
            scent = cells[p.y()][p.x()].getScent();
            if (scent > best) {
                best = scent;
                direction = directionAfterRightTurn(direction);
            }
            int random = qrand() % 100;
            if (random < 10)
                direction = directionAfterLeftTurn(direction);
            else if (random < 20)
                direction = directionAfterRightTurn(direction);

            p = pointAfterForwardMove(direction);
            moveTo(p);

            return true;
        }
    }

    if (cells[location.y()][location.x()].hasFood()) {
        hasFood = cells[location.y()][location.x()].takeFood();
        if (hasFood) {
            direction = oppositeDirection(direction);
            ellipse->setPen(QPen(hasFoodColor));
            ellipse->setBrush(QBrush(hasFoodColor));
            foodSource = QPoint(location);
        }
        return update(cells);
    } else {
        double leftPh = 0, rightPh = 0, straightPh = 0;
        QPoint p = pointAfterForwardMove(direction);
        straightPh = cells[p.y()][p.x()].getPheremone();
        p = pointAfterForwardMove(directionAfterLeftTurn(direction));
        leftPh = cells[p.y()][p.x()].getPheremone();
        p = pointAfterForwardMove(directionAfterRightTurn(direction));
        rightPh = cells[p.y()][p.x()].getPheremone();

        if (straightPh <= 00 && rightPh <= 0.0 && leftPh <= 0.0) {
            moveRandom();
            return false;
        }

        if (leftPh > rightPh && leftPh > straightPh)
            direction = directionAfterLeftTurn(direction);
        else if (rightPh > straightPh)
            direction = directionAfterRightTurn(direction);
        p = pointAfterForwardMove(direction);
        moveTo(p);
        return false;
    }

}

Direction Ant::getDirection() const
{
    return direction;
}

QPoint Ant::getFoodSource()
{
    return foodSource;
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

void Ant::moveTo(QPoint &p)
{
    if (p == location) {
        moveRandom();
        return;
    }
    location = p;
    ellipse->setRect(LOC_HELPER(location,ANT_RAD));
}

void Ant::moveTowards(const QPoint &p)
{
    int dx = p.x() - location.x(), dy = p.y() - location.y();
    QPoint dest(location);
    if (abs(dx) > abs(dy)) {
        dest.setX(location.x() + qBound(-1, dx, 1));
        direction = (dx > 0) ? East : West;
    } else {
        dest.setY(location.y() + qBound(-1, dy, 1));
        direction = (dy > 0) ? North : South;
    }
    moveTo(dest);
}

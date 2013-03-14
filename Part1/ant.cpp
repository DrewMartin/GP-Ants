#include "ant.h"
#include <QDebug>

const QColor Ant::color(255, 0, 0);
const QColor Ant::hasFoodColor(255, 180, 0);

Ant::Ant(QPoint &location) :
    Entity(location),
    initialLocation(location),
    hasFood(false)
{
    switch(qrand() % 4) {
    case 0: direction = North; break;
    case 1: direction = East; break;
    case 2: direction = South; break;
    default: direction = West; break;
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

bool Ant::update(Cell cells[MAX_Y][MAX_X])
{
    if (hasFood) {
        if (cells[location.y()][location.x()].atAnthill()) {
            hasFood = false;
            ellipse->setPen(QPen(color));
            ellipse->setBrush(QBrush(color));
        } else {
            direction = North;
            int bestScent = -1;

            QPoint move(location.x(), location.y()+1), bestMove;
            if (isValidPoint(move)) {
                bestScent = cells[move.x()][move.y()].getScent();
                bestMove = move;
            }

            move = QPoint(location.x()+1, location.y());
            if (isValidPoint(move) && cells[move.x()][move.y()].getScent() > bestScent) {
                bestScent = cells[move.x()][move.y()].getScent();
                bestMove = move;
                direction = East;
            }

            move = QPoint(location.x(), location.y()-1);
            if (isValidPoint(move) && cells[move.x()][move.y()].getScent() > bestScent) {
                bestScent = cells[move.x()][move.y()].getScent();
                bestMove = move;
                direction = South;
            }

            move = QPoint(location.x()-1, location.y());
            if (isValidPoint(move) && cells[move.x()][move.y()].getScent() > bestScent) {
                bestScent = cells[move.x()][move.y()].getScent();
                bestMove = move;
                direction = West;
            }

            moveTo(bestMove);
            return true;
        }
    }

    if (cells[location.y()][location.x()].hasFood()) {
        hasFood = cells[location.y()][location.x()].takeFood();
        if (hasFood) {
            ellipse->setPen(QPen(hasFoodColor));
            ellipse->setBrush(QBrush(hasFoodColor));
            foodSource = QPoint(location);
        }
        return update(cells);
    } else if (cells[location.y()][location.x()].hasPheremone()){
        moveTowards(cells[location.y()][location.x()].getPheremone()->getDestination());
    }
    moveRandom();
    return false;

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
    do {
        random = qrand() % 100;
        targetDir = direction;
        if (random >= 75)
            targetDir = directionAfterLeftTurn(direction);
        else if (random >= 50)
            targetDir = directionAfterRightTurn(direction);
        dest = pointAfterForwardMove(targetDir);
    } while (!isValidPoint(dest));
    direction = targetDir;
    moveTo(dest);
}

bool Ant::isValidPoint(QPoint &p)
{
    return p.x() >= 0 && p.x() < MAX_X &&
            p.y() >= 0 && p.y() < MAX_Y;
}

QPoint Ant::pointAfterForwardMove(Direction dir)
{
    QPoint dest(location);
    switch (dir) {
    case North: dest.setY(dest.y() + 1); break;
    case East: dest.setX(dest.x() + 1); break;
    case South: dest.setY(dest.y() - 1); break;
    default: dest.setX(dest.x() - 1); break;
    }
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

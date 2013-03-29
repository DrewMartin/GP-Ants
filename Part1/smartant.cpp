#include "smartant.h"

SmartAnt::SmartAnt(QPoint &location) :
    Ant(location)
{
}

SmartAnt::~SmartAnt()
{
}

bool SmartAnt::update(QList<QList<Cell> > &cells)
{
    if (hasFood) {
        if (cells[location.y()][location.x()].atAnthill()) {
            dropFood();
        } else {
            double scent, best;
            Direction newDir = direction;
            QPoint p = pointAfterForwardMove(direction);
            scent = best = cells[p.y()][p.x()].getScent();

            p = pointAfterForwardMove(directionAfterLeftTurn(direction));
            scent = cells[p.y()][p.x()].getScent();
            if (scent > best) {
                best = scent;
                newDir = directionAfterLeftTurn(direction);
            }

            p = pointAfterForwardMove(directionAfterRightTurn(direction));
            scent = cells[p.y()][p.x()].getScent();
            if (scent > best) {
                best = scent;
                newDir = directionAfterRightTurn(direction);
            }
            int random = qrand() % 100;
            if (random < 10)
                newDir = directionAfterLeftTurn(newDir);
            else if (random < 20)
                newDir = directionAfterRightTurn(newDir);

            direction = newDir;
            p = pointAfterForwardMove(newDir);
            moveTo(p);

            return true;
        }
    }

    if (cells[location.y()][location.x()].hasFood()) {
        if (cells[location.y()][location.x()].takeFood())
            takeFood();
        return update(cells);
    } else {
        double leftPh = 0, rightPh = 0, straightPh = 0;
        QPoint p = pointAfterForwardMove(direction);
        straightPh = cells[p.y()][p.x()].getPheremone();
        p = pointAfterForwardMove(directionAfterLeftTurn(direction));
        leftPh = cells[p.y()][p.x()].getPheremone();
        p = pointAfterForwardMove(directionAfterRightTurn(direction));
        rightPh = cells[p.y()][p.x()].getPheremone();

        if (straightPh <= 0.0 && rightPh <= 0.0 && leftPh <= 0.0) {
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

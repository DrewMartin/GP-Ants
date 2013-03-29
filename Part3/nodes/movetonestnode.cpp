#include "movetonestnode.h"

MoveToNestNode::MoveToNestNode()
{
}

void MoveToNestNode::eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant)
{
    double scent, best;
    Direction direction = ant->getDirection(), newDir = direction;
    QPoint p = ant->pointAfterForwardMove(direction);
    scent = best = cells[p.y()][p.x()].getScent();

    p = ant->pointAfterForwardMove(directionAfterLeftTurn(direction));
    scent = cells[p.y()][p.x()].getScent();
    if (scent > best) {
        best = scent;
        newDir = directionAfterLeftTurn(direction);
    }

    p = ant->pointAfterForwardMove(directionAfterRightTurn(direction));
    scent = cells[p.y()][p.x()].getScent();
    if (scent > best) {
        best = scent;
        newDir = directionAfterRightTurn(direction);
    }

    ant->setDirection(newDir);
    p = ant->pointAfterForwardMove(newDir);
    ant->moveTo(p);
    dropFoodAfterMove(cells, ant);
}

QString MoveToNestNode::toString()
{
    return "MoveToNest";
}


MoveToNestNode *MoveToNestNode::copy()
{
    return new MoveToNestNode();
}

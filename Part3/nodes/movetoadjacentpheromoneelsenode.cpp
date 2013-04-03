#include "movetoadjacentpheromoneelsenode.h"

MoveToAdjacentPheromoneElseNode::MoveToAdjacentPheromoneElseNode(QSP<AntNode> elseNode)
{
    children.append(elseNode);
}

void MoveToAdjacentPheromoneElseNode::eval(QList<QList<Cell> > &cells, const QSP<GPAnt> &ant)
{
    double leftPh = 0.0, rightPh = 0.0, straightPh = 0.0;
    Direction antDir = ant->getDirection();
    QPoint p = ant->pointAfterForwardMove(antDir);
    straightPh = cells[p.y()][p.x()].getPheremone();
    p = ant->pointAfterForwardMove(directionAfterLeftTurn(antDir));
    leftPh = cells[p.y()][p.x()].getPheremone();
    p = ant->pointAfterForwardMove(directionAfterRightTurn(antDir));
    rightPh = cells[p.y()][p.x()].getPheremone();

    if (straightPh <= 0.0 && rightPh <= 0.0 && leftPh <= 0.0) {
        children.at(0).staticCast<AntNode>()->eval(cells, ant);
        return;
    }
    if (leftPh > rightPh && leftPh > straightPh)
        antDir = directionAfterLeftTurn(antDir);
    else if (rightPh > straightPh)
        antDir = directionAfterRightTurn(antDir);

    ant->setDirection(antDir);
}

QString MoveToAdjacentPheromoneElseNode::toString()
{
    return QString("%1(%2)").arg(MOVE_TO_PH_STRING).arg(children.at(0)->toString());
}


MoveToAdjacentPheromoneElseNode *MoveToAdjacentPheromoneElseNode::copy()
{
    return new MoveToAdjacentPheromoneElseNode();
}

MoveToAdjacentPheromoneElseNode::MoveToAdjacentPheromoneElseNode()
{
}

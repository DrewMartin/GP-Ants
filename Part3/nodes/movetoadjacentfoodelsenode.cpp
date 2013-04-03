#include "movetoadjacentfoodelsenode.h"

MoveToAdjacentFoodElseNode::MoveToAdjacentFoodElseNode(QSharedPointer<AntNode> elseNode)
{
    children.append(elseNode);
}

void MoveToAdjacentFoodElseNode::eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant)
{
    Direction antDir = ant->getDirection();
    QPoint p = ant->pointAfterForwardMove(antDir);
    if (cells[p.y()][p.x()].hasFood()) {
        return;
    }

    p = ant->pointAfterForwardMove(directionAfterLeftTurn(antDir));
    if (cells[p.y()][p.x()].hasFood()) {
        ant->setDirection(directionAfterLeftTurn(antDir));
        return;
    }

    p = ant->pointAfterForwardMove(directionAfterRightTurn(antDir));
    if (cells[p.y()][p.x()].hasFood()) {
        ant->setDirection(directionAfterRightTurn(antDir));
        return;
    }

    children.at(0).staticCast<AntNode>()->eval(cells, ant);
}


MoveToAdjacentFoodElseNode *MoveToAdjacentFoodElseNode::copy()
{
    return new MoveToAdjacentFoodElseNode();
}

MoveToAdjacentFoodElseNode::MoveToAdjacentFoodElseNode()
{

}


QString MoveToAdjacentFoodElseNode::toString()
{
    return QString("%1(%2)").arg(MOVE_TO_FOOD_STRING).arg(children.at(0)->toString());
}

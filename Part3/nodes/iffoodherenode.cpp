#include "iffoodherenode.h"

IfFoodHereNode::IfFoodHereNode(QSharedPointer<AntNode> thenNode, QSharedPointer<AntNode> elseNode)
{
    children.append(thenNode);
    children.append(elseNode);
}

void IfFoodHereNode::eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant)
{
    QPoint antLoc = ant->getLocation();
    if (cells[antLoc.y()][antLoc.x()].hasFood())
        children.at(0).staticCast<AntNode>()->eval(cells, ant);
    else
        children.at(1).staticCast<AntNode>()->eval(cells, ant);
}

QString IfFoodHereNode::toString()
{
    return QString("IfFoodHere(%1, %2)").arg(children.at(0)->toString()).arg(children.at(1)->toString());
}


IfFoodHereNode *IfFoodHereNode::copy()
{
    return new IfFoodHereNode();
}

IfFoodHereNode::IfFoodHereNode()
{
}

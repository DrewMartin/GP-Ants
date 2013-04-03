#include "ifcarryingfoodnode.h"

IfCarryingFoodNode::IfCarryingFoodNode(QSP<AntNode> thenNode, QSP<AntNode> elseNode)
{
    children.append(thenNode);
    children.append(elseNode);
}

IfCarryingFoodNode::IfCarryingFoodNode()
{

}

void IfCarryingFoodNode::eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant)
{
    if (ant->antHasFood())
        children.at(0).staticCast<AntNode>()->eval(cells, ant);
    else
        children.at(1).staticCast<AntNode>()->eval(cells, ant);
}

QString IfCarryingFoodNode::toString()
{
    return QString("%1(%2, %3)").arg(IF_CARRYING_STRING).arg(children.at(0)->toString()).arg(children.at(1)->toString());
}


IfCarryingFoodNode *IfCarryingFoodNode::copy()
{
    return new IfCarryingFoodNode();
}

#include "pickupnode.h"

PickUpNode::PickUpNode()
{
}

void PickUpNode::eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant)
{
    if (ant->antHasFood())
        return;
    QPoint p = ant->getLocation();
    if (cells[p.y()][p.x()].takeFood())
        ant->takeFood();
}

QString PickUpNode::toString()
{
    return PICKUP_STRING;
}


PickUpNode *PickUpNode::copy()
{
    return new PickUpNode();
}

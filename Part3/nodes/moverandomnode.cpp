#include "moverandomnode.h"

MoveRandomNode::MoveRandomNode()
{
}

void MoveRandomNode::eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant)
{
    ant->moveRandom();
    dropFoodAfterMove(cells, ant);
}

QString MoveRandomNode::toString()
{
    return "MoveRandom";
}


MoveRandomNode *MoveRandomNode::copy()
{
    return new MoveRandomNode();
}

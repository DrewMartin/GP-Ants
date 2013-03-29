#include "moverandomnode.h"

MoveRandomNode::MoveRandomNode()
{
}

void MoveRandomNode::eval(QList<QList<Cell> > &/*cells*/, const QSharedPointer<GPAnt> &ant)
{
    ant->turnRandom();
}

QString MoveRandomNode::toString()
{
    return "MoveRandom";
}


MoveRandomNode *MoveRandomNode::copy()
{
    return new MoveRandomNode();
}

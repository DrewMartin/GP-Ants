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
    return QString(MOVE_RAND_STRING);
}


MoveRandomNode *MoveRandomNode::copy()
{
    return new MoveRandomNode();
}

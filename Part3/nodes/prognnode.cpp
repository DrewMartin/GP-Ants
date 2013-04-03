#include "prognnode.h"

PrognNode::PrognNode(QSP<AntNode> c1, QSP<AntNode> c2)
{
    children.append(c1);
    children.append(c2);
}

void PrognNode::eval(QList<QList<Cell> > &cells, const QSP<GPAnt> &ant)
{
    children.at(0).staticCast<AntNode>()->eval(cells, ant);
    children.at(1).staticCast<AntNode>()->eval(cells, ant);
}

QString PrognNode::toString()
{
    return QString("%1(%2, %3)").arg(PROGN_STRING).arg(children.at(0)->toString()).arg(children.at(1)->toString());
}

PrognNode *PrognNode::copy()
{
    return new PrognNode();
}

PrognNode::PrognNode()
{
}

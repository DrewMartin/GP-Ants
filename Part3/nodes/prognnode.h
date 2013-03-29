#ifndef PROGNNODE_H
#define PROGNNODE_H

#include "antnode.h"

class PrognNode : public AntNode
{
public:
    PrognNode(QSP<AntNode> child1, QSP<AntNode> child2);

    virtual void eval(QList<QList<Cell> > &cells, const QSP<GPAnt> &ant);

    virtual QString toString();

protected:
    virtual PrognNode* copy();

private:
    PrognNode();
};

#endif // PROGNNODE_H

#ifndef ANTNODE_H
#define ANTNODE_H

#include "common/node.h"
#include "common/cell.h"
#include "gpant.h"

class AntNode : public Node
{
public:
    AntNode();
    virtual ~AntNode();

    static QSP<AntNode> generateFullTree(int maxDepth);
    static QSP<AntNode> generateGrowTree(int maxDepth);

    virtual void eval(QList<QList<Cell> > &cells, const QSP<GPAnt> &ant) = 0;

    int getScore();
    bool isScored();

    void setScore(int val);

protected:
    virtual QSP<Node> generateSubtree(int maxHeight);
    void dropFoodAfterMove(QList<QList<Cell> > &cells, const QSP<GPAnt> &ant);

private:
    static QSP<AntNode> generateTree(int height, bool full);
    int myScore;
    bool scored;
};

#endif // ANTNODE_H

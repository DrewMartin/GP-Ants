#ifndef MATHNODE_H
#define MATHNODE_H

#include "common/node.h"
#include "common/common.h"
#include "operators.h"



class MathNode : public Node
{
public:
    MathNode();
    virtual ~MathNode();

    static QSP<MathNode> generateFullTree(int maxDepth);
    static QSP<MathNode> generateGrowTree(int maxDepth);

    virtual double eval(double var) = 0;

    double getScore(QList<QPointF> &points);
    double getScore();

protected:
    virtual QSP<Node> generateSubtree(int maxHeight);

private:
    static QSP<MathNode> generateTree(int height, bool full);

    double myScore;
    bool scored;
};

#endif // MATHNODE_H

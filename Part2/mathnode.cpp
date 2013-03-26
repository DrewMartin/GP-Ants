#include "mathnode.h"
#include <QDebug>
#include <QPointF>

#include "constantnode.h"
#include "variablenode.h"
#include "singleargnode.h"
#include "twoargnode.h"

#include <qmath.h>

static SingleArgOp singleArgOps[] = { qSin, qCos };
static TwoArgOp twoArgOps[] = { mult, add, sub, divide, power };

#define SINGLE_ARG_OP_COUNT 2
#define TWO_ARG_OP_COUNT 5
#define TOTAL_ARG_COUNT (SINGLE_ARG_OP_COUNT + TWO_ARG_OP_COUNT)

MathNode::MathNode() :
    scored(false)
{
}

MathNode::~MathNode()
{
}

QSP<MathNode> MathNode::generateFullTree(int depth)
{
    QSP<MathNode> tree =  generateTree(depth, true);
    tree->updateStats();
    return tree;
}

QSP<MathNode> MathNode::generateGrowTree(int depth)
{
    QSP<MathNode> tree = generateTree(depth, false);
    tree->updateStats();
    return tree;
}

double MathNode::getScore(QList<QPointF> &points)
{
    if (scored)
        return myScore;
    double score = 0;
    double diff;
    QPointF p;
    for (int i = 0; i < points.length(); i++) {
        p = points.at(i);
        diff = eval(p.x());
        diff = qAbs(diff - p.y());
        if (diff <= PRECISION)
            diff = 0;
        score += diff;
    }
    myScore = score;
    scored = true;
    return score;
}

double MathNode::getScore()
{
    if (!scored)
        throw QString("Not scored yet");
    return myScore;
}

QSP<Node<double, double> > MathNode::generateSubtree(int maxHeight)
{
    return generateTree(maxHeight, false);
}

QSP<MathNode> MathNode::generateTree(int height, bool full)
{
    int random = RAND_INT(TOTAL_ARG_COUNT + 2);
    if (height <= 0 || (random < 2 && !full)) {
        if (RAND_INT(100) < 50)
            return QSP<MathNode>(new VariableNode());
        else
            return QSP<MathNode>(new ConstantNode(RAND_INT(MAX_CONSTANT) + 1));
    }

    random = RAND_INT(TOTAL_ARG_COUNT);
    if (random < SINGLE_ARG_OP_COUNT) {
        return QSP<MathNode>(new SingleArgNode(generateTree(height -  1, full),
                                               singleArgOps[random]));
    } else {
        return QSP<MathNode>(new TwoArgNode(generateTree(height - 1, full),
                                            generateTree(height - 1, full),
                                            twoArgOps[random - SINGLE_ARG_OP_COUNT]));
    }
}

#include "antnode.h"

#include "droppheromonenode.h"
#include "ifcarryingfoodnode.h"
#include "iffoodherenode.h"
#include "moverandomnode.h"
#include "movetoadjacentpheromoneelsenode.h"
#include "movetonestnode.h"
#include "pickupnode.h"
#include "prognnode.h"

AntNode::AntNode() :
    scored(false)
{
}

AntNode::~AntNode()
{
}

QSP<AntNode> AntNode::generateFullTree(int maxDepth)
{
    QSP<AntNode> tree =  generateTree(maxDepth, true);
    tree->updateStats();
    return tree;
}

QSP<AntNode> AntNode::generateGrowTree(int maxDepth)
{
    QSP<AntNode> tree =  generateTree(maxDepth, false);
    tree->updateStats();
    return tree;
}

int AntNode::getScore()
{
    return myScore;
}

bool AntNode::isScored()
{
    return scored;
}

void AntNode::setScore(int val)
{
    myScore = val;
    scored = true;
}

QSharedPointer<Node> AntNode::generateSubtree(int maxHeight)
{
    return generateTree(maxHeight, false);
}

#define TWO_ARGS generateTree(height -  1, full), generateTree(height -  1, full)
#define ONE_ARG generateTree(height -  1, full)

QSharedPointer<AntNode> AntNode::generateTree(int height, bool full)
{
    int random = RAND_INT(8);
    if (height <= 0 || (random < 2 && !full)) {
        random = random % 4;

        switch(random) {
        case 0:
            return QSP<AntNode>(new DropPheromoneNode());
        case 1:
            return QSP<AntNode>(new MoveRandomNode());
        case 2:
            return QSP<AntNode>(new MoveToNestNode());
        default:
            return QSP<AntNode>(new PickUpNode());
        }
    }

    random = random % 4;
    switch(random) {
    case 0:
        return QSP<AntNode>(new IfFoodHereNode(TWO_ARGS));
    case 1:
        return QSP<AntNode>(new IfCarryingFoodNode(TWO_ARGS));
    case 2:
        return QSP<AntNode>(new MoveToAdjacentPheromoneElseNode(ONE_ARG));
    default:
        return QSP<AntNode>(new PrognNode(TWO_ARGS));
    }

}

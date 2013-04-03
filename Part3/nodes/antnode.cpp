#include "antnode.h"

#include <QRegExp>

#include "droppheromonenode.h"
#include "ifcarryingfoodnode.h"
#include "iffoodherenode.h"
#include "moverandomnode.h"
#include "movetoadjacentpheromoneelsenode.h"
#include "movetonestnode.h"
#include "pickupnode.h"
#include "prognnode.h"
#include "movetoadjacentfoodelsenode.h"

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

#define FIRST_TOKEN "^(\\w+)([(), ]+|$)"

QSharedPointer<AntNode> AntNode::fromString(QString &source)
{
    QRegExp rx(FIRST_TOKEN);
    if (rx.indexIn(source) < 0)
        throw "bad";
    QStringList tokens = rx.capturedTexts();

    QString firstToken = tokens.at(1);
    source.remove(rx);

    if (firstToken == DROP_STRING) {
        return QSP<AntNode>(new DropPheromoneNode());
    } else if (firstToken == MOVE_RAND_STRING) {
        return QSP<AntNode>(new MoveRandomNode());
    } else if (firstToken == MOVE_TO_NEST_STRING) {
        return QSP<AntNode>(new MoveToNestNode());
    } else if (firstToken == PICKUP_STRING) {
        return QSP<AntNode>(new PickUpNode());
    }

    QSP<AntNode> firstChild = fromString(source);
    if (firstToken == MOVE_TO_PH_STRING) {
        return QSP<AntNode>(new MoveToAdjacentPheromoneElseNode(firstChild));
    } else if (firstToken == MOVE_TO_FOOD_STRING) {
        return QSP<AntNode>(new MoveToAdjacentFoodElseNode(firstChild));
    }

    QSP<AntNode> secondChild = fromString(source);
    if (firstToken == IF_FOOD_HERE_STRING) {
        return QSP<AntNode>(new IfFoodHereNode(firstChild, secondChild));
    } else if (firstToken == IF_CARRYING_STRING) {
        return QSP<AntNode>(new IfCarryingFoodNode(firstChild, secondChild));
    } else if (firstToken == PROGN_STRING) {
        return QSP<AntNode>(new PrognNode(firstChild, secondChild));
    }

    throw "bad";
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
    int random = RAND_INT(9);
    if (height <= 0 || (random < 4 && !full)) {
        random = RAND_INT(4);

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

    random = RAND_INT(5);
    switch(random) {
    case 0:
        return QSP<AntNode>(new IfFoodHereNode(TWO_ARGS));
    case 1:
        return QSP<AntNode>(new IfCarryingFoodNode(TWO_ARGS));
    case 2:
        return QSP<AntNode>(new MoveToAdjacentPheromoneElseNode(ONE_ARG));
    case 3:
        return QSP<AntNode>(new MoveToAdjacentFoodElseNode(ONE_ARG));
    default:
        return QSP<AntNode>(new PrognNode(TWO_ARGS));
    }

}

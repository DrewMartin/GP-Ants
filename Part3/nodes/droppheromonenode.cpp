#include "droppheromonenode.h"
#include "options.h"

DropPheromoneNode::DropPheromoneNode()
{
}

void DropPheromoneNode::eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant)
{
    QPoint antLoc = ant->getLocation(), loc;
    int rad = Options::getInstance()->getPheromoneRadius();
    if (rad > 0) {
        CIRCLE_LOOP_HELPER(antLoc.x(), antLoc.y(), rad) {
            loc = QPoint(col, row);
            cells[row][col].addPheremone(50 - sqDist * 50/sqRad);
        }
    }
}

QString DropPheromoneNode::toString()
{
    return QString("DropPheromone");
}

DropPheromoneNode *DropPheromoneNode::copy()
{
    return new DropPheromoneNode();
}

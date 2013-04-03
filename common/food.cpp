#include "food.h"
#include <QDebug>


Food::Food(QPoint &location, int quantity) :
    Entity(location),
    initialQuantity(quantity),
    quantity(quantity),
    color(0, 0, 255),
    ellipse(NULL)
{
    redraw();
}

Food::~Food()
{
    if (ellipse)
        delete ellipse;
}

int Food::getFoodLeft()
{
    return quantity;
}

bool Food::takeFood()
{
    quantity = qMax(0, quantity - 1);
    redraw();
    return hasFood();
}

bool Food::hasFood()
{
    return quantity > 0;
}

QGraphicsItem *Food::getGraphicsItem()
{
    if (!ellipse) {
        ellipse = new QGraphicsEllipseItem(LOC_HELPER(location, FOOD_RAD));
        ellipse->setPen(QPen(QColor(0, 0, 0, 0)));
        ellipse->setFlag(QGraphicsItem::ItemClipsToShape);
        ellipse->setZValue(1);
        redraw();
    }
    return ellipse;
}


void Food::reset()
{
    if (ellipse)
        ellipse->setVisible(true);

    quantity = initialQuantity;
    redraw();
}

void Food::redraw()
{
    if (!ellipse) {
        return;
    }

    if (quantity == 0) {
        ellipse->setVisible(false);
    } else {
        int val = qMin(255 - quantity*255/MAX_FOOD, 250);
        color.setRed(val);
        color.setGreen(val);
    }
    ellipse->setBrush(QBrush(color));
}

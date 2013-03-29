#ifndef FOOD_H
#define FOOD_H

#include "entity.h"
#include <QGraphicsEllipseItem>


class Food : public Entity
{
public:
    Food(QPoint &location, int quantity);
    virtual ~Food();

    int getFoodLeft();
    bool takeFood();
    bool hasFood();

    virtual QGraphicsItem *getGraphicsItem();
    virtual void reset();

private:
    void redraw();

    const int initialQuantity;
    int quantity;
    QColor color;
    QGraphicsEllipseItem *ellipse;
};

#endif // FOOD_H

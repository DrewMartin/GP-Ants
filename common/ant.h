#ifndef ANT_H
#define ANT_H

#include "common/entity.h"
#include "common/cell.h"
#include <QList>

#define NORMALIZE_POINT(p) p.setX((p.x() + MAX_X) % MAX_X); p.setY((p.y() + MAX_Y) % MAX_Y)

class Ant : public Entity
{
public:
    Ant(QPoint &location);
    virtual ~Ant();

    virtual QGraphicsItem *getGraphicsItem();
    virtual void reset();

    Direction getDirection() const;

    QPoint pointAfterForwardMove(Direction dir);

    void takeFood();
    void dropFood();

protected:
    void moveTo(QPoint &p);
    void moveRandom();

    const QPoint initialLocation;

    QGraphicsEllipseItem *ellipse;
    bool hasFood;
    Direction direction;

    const static QColor color;
    const static QColor hasFoodColor;
};

#endif // ANT_H

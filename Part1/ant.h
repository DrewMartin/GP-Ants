#ifndef ANT_H
#define ANT_H

#include "entity.h"
#include "cell.h"
#include <QList>

#define NORMALIZE_POINT(p) p.setX((p.x() + MAX_X) % MAX_X); p.setY((p.y() + MAX_Y) % MAX_Y)

class Ant : public Entity
{
public:
    Ant(QPoint &location);
    virtual ~Ant();

    virtual QGraphicsItem *getGraphicsItem();
    virtual void reset();

    bool update(QList<QList<Cell> > &cells);
    Direction getDirection() const;
    QPoint getFoodSource();


private:
    void moveTo(QPoint &p);
    void moveTowards(const QPoint &p);
    void moveRandom();
    QPoint pointAfterForwardMove(Direction dir);

    const QPoint initialLocation;

    QPoint foodSource;
    QGraphicsEllipseItem *ellipse;
    bool hasFood;
    Direction direction;

    const static QColor color;
    const static QColor hasFoodColor;
};

#endif // ANT_H

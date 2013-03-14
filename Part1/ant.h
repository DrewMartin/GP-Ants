#ifndef ANT_H
#define ANT_H

#include "entity.h"
#include "cell.h"

class Ant : public Entity
{
public:
    Ant(QPoint &location);
    virtual ~Ant();

    virtual QGraphicsItem *getGraphicsItem();
    virtual void reset();

    bool update(Cell cells[MAX_Y][MAX_Y]);
    Direction getDirection() const;
    QPoint getFoodSource();


private:
    void moveTo(QPoint &p);
    void moveTowards(const QPoint &p);
    void moveRandom();
    bool isValidPoint(QPoint &p);
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

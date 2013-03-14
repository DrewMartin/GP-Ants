#ifndef CELL_H
#define CELL_H

#include "food.h"
#include "pheremone.h"

class Cell : public Entity
{
public:
    Cell(QPoint &location);
    virtual ~Cell();

    bool hasFood() const;
    bool atAnthill() const;
    bool takeFood();
    int getScent() const;

    bool addPheremone(int amount);
    void update();
    void clear();

    virtual QGraphicsItem *getGraphicsItem();
    virtual void reset();

    bool hasPheremone() const;
    int getPheremone();

    void setAnthill();
    void setFood(QSP<Food> food);
    void setScent(int val);

    static void setDecay(double decay);

private:
    void redraw();

    QSP<Food> food;
    bool anthill;
    QColor color;
    int scent;
    int pheromone;
    QGraphicsRectItem *rect;

    static double decay;
};

#endif // CELL_H
